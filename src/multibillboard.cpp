#include "multibillboard.h"

#include <QOpenGLFunctions>
#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>
#include <iostream>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_4_0_Core>
#include <QQuickWindow>

using namespace std;

MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    firstPaint(true),
    m_useGeometryShader(false),
    m_dataSource(0),
    m_firstVertexBuild(true),
    bConnectedToOpenGLContextSignal(false),
    m_texture2D(0)
{
    m_effect = new CustomEffect();
}

bool MultiBillboard::hasGeometryShaderSupport(QGLPainter *painter) {
    if(firstPaint) {
        const QSurfaceFormat& format = painter->context()->format();
        qDebug() << "MultiBillboard: OpenGL version " << format.majorVersion() << "." << format.minorVersion();
        if ( ! (format.majorVersion() > 3 || (format.majorVersion() == 3 && format.minorVersion() >= 3)) )
        {
            qDebug("MultiBillboard: Geometry shader requires OpenGL >= 3.3. Falling back to CPU billboards.");
            m_useGeometryShader = false;
        } else {
            m_useGeometryShader = true;
        }
        if ( ! (format.majorVersion() > 4 || (format.majorVersion() == 4 && format.minorVersion() >= 0)) )
        {
            qDebug("MultiBillboard: Periodic boundary conditions and geometry shader in combination requires OpenGL >= 4.0. Disabling the use of periodic boundary conditions.");
            setHasPeriodicCopies(false);
            m_effect->setPeriodicCopiesAllowed(false);
        }
        firstPaint = false;
    }
    return m_useGeometryShader;
}

void MultiBillboard::setTexture(const QUrl &value)
{
    if(m_texture != value && !value.isEmpty()) {
        if(m_texture2D) {
            m_texture2D->cleanupResources();
        }
        m_texture2D = new QGLTexture2D(this);
        m_texture2D->setUrl(value);
    }
}

void MultiBillboard::drawItem(QGLPainter *painter) {
    if(m_dataSource) {
        if(hasGeometryShaderSupport(painter)) {
            drawGeometryShaderBillboards(painter);
        } else {
            drawCPUBillboards(painter);
        }
    }
}

void MultiBillboard::drawEffectSetup(QGLPainter *painter, bool &viewportBlend, bool &effectBlend)
{
    m_effect->setUseGeometryShader(hasGeometryShaderSupport(painter));
    if(m_texture2D) {
        m_texture2D->bind();
    }
    // Setting our custom effect must be done before passing any data
    painter->setUserEffect(m_effect);
}

void MultiBillboard::drawEffectCleanup(QGLPainter *painter, bool &viewportBlend, bool &effectBlend)
{
    if(m_texture2D) {
        // Uncomment this line to waste GPU bandwidth and avoid the segfault at the
        // end of running your program.
        // This basically cleans up all resources used by the texture for each drawed
        // frame. This happens while there is still a GL context, and so avoids
        // the annoying segfault.
//        m_texture2D->cleanupResources();
    }
    painter->setStandardEffect(QGL::FlatColor);
    painter->setColor(Qt::white);
    painter->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (viewportBlend != effectBlend) {
        if (effectBlend)
            glDisable(GL_BLEND);
        else
            glEnable(GL_BLEND);
    }
}

void MultiBillboard::handleOpenglContextIsAboutToBeDestroyedYeah()
{
    m_texture2D->cleanupResources();
}

void MultiBillboard::drawGeometryShaderBillboards(QGLPainter *painter) {
    if(m_dataSource->dataBundles() == 0 || m_dataSource->dataBundles()->count() == 0) {
        return;
    }

    painter->clearAttributes();

    // After the effect has been set, we may start passing data (perhaps the effect resets some painter props?)

    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
    QVector3D right;
    right.setX(modelViewMatrix(0,0));
    right.setY(modelViewMatrix(0,1));
    right.setZ(modelViewMatrix(0,2));
    QVector3D up;
    up.setX(modelViewMatrix(1,0));
    up.setY(modelViewMatrix(1,1));
    up.setZ(modelViewMatrix(1,2));
    QVector3D normal(QVector3D::crossProduct(right, up));
    painter->glDisableVertexAttribArray(GLuint(QGL::Normal));
    painter->glVertexAttrib3f(GLuint(QGL::Normal), normal.x(), normal.y(), normal.z());

    for(DataBundle* bundle : *(m_dataSource->dataBundles())) {
        // Set the rest of the vertex bundle (basically only positions)
        m_effect->setSize(bundle->size());
        m_effect->setColor(bundle->color());
        m_effect->setSystemSize(bundle->systemSize());
        m_effect->update(painter, QGLPainter::UpdateColor);
        painter->setVertexBundle(bundle->vertexBundle());
        painter->draw(QGL::DrawingMode(QGL::Points), bundle->vertexBundle().vertexCount());
    }
}

void MultiBillboard::drawCPUBillboards(QGLPainter *painter) {
//    QGLVertexBundle vertexBundle;
//    QGLIndexBuffer indexBuffer;

//    vertices.clear();
//    indices.clear();
//    texCoords.clear();
//    colorArray.clear();

//    const QArray<QVector3D> &positions = m_dataSource->getPositions();
//    const QArray<QColor4ub> &colors = m_dataSource->getColors();
//    const QArray<QVector2D > &sizes = m_dataSource->getSizes();

//    vertices.reserve(4*positions.count());
//    texCoords.reserve(4*positions.count());

//    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
//    QVector3D right;
//    right.setX(modelViewMatrix(0,0));
//    right.setY(modelViewMatrix(0,1));
//    right.setZ(modelViewMatrix(0,2));
//    QVector3D up;
//    up.setX(modelViewMatrix(1,0));
//    up.setY(modelViewMatrix(1,1));
//    up.setZ(modelViewMatrix(1,2));
//    QVector3D a,b,c,d;

//    QVector3D aOffset_x =  - right * 0.5;
//    QVector3D aOffset_y =  - up * 0.5;

//    QVector3D bOffset_x =  right * 0.5;
//    QVector3D bOffset_y =  - up * 0.5;

//    QVector3D cOffset_x =  right * 0.5;
//    QVector3D cOffset_y =  up * 0.5;

//    QVector3D dOffset_x =  - right * 0.5;
//    QVector3D dOffset_y =  up * 0.5;
//    QVector2D ta(0,0);
//    QVector2D tb(0,1);
//    QVector2D tc(1,1);
//    QVector2D td(1,0);

//    QVector3D normal(QVector3D::crossProduct(right, up));

//    for(int i = 0; i <positions.count(); i++) {
//        const QVector3D &center = positions.at(i);
//        a = center + sizes.at(i).y()*aOffset_x + sizes.at(i).x()*aOffset_y;
//        b = center + sizes.at(i).y()*bOffset_x + sizes.at(i).x()*bOffset_y;
//        c = center + sizes.at(i).y()*cOffset_x + sizes.at(i).x()*cOffset_y;
//        d = center + sizes.at(i).y()*dOffset_x + sizes.at(i).x()*dOffset_y;
//        vertices.append(a, b, c, d);
//        colorArray.append(colors.at(i), colors.at(i), colors.at(i), colors.at(i));
//        texCoords.append(ta, tb, tc, td);
//        indices.append(i*4 + 0, i*4 + 1, i*4 + 2);
//        indices.append(i*4 + 2, i*4 + 3, i*4 + 0);
//    }
//    vertexBundle.addAttribute(QGL::Position, vertices);
//    vertexBundle.addAttribute(QGL::TextureCoord0, texCoords);
//    vertexBundle.addAttribute(QGL::Color, colorArray);
//    indexBuffer.setIndexes(indices);

//    painter->clearAttributes();
//    // Set up normal attributes to use only one element
//    painter->glDisableVertexAttribArray(GLuint(QGL::Normal));
//    painter->glVertexAttrib3f(GLuint(QGL::Normal), normal.x(), normal.y(), normal.z());

//    // Set the rest of the vertex bundle (basically only positions)
//    painter->setVertexBundle(vertexBundle);
//    painter->draw(QGL::DrawingMode(QGL::Triangles), indexBuffer, 0, indexBuffer.indexCount());
}

MultiBillboard::~MultiBillboard()
{
    if(m_effect) {
        delete m_effect;
    }
    if(m_texture2D) {
        delete m_texture2D;
    }
}
