#include "multibillboard.h"

#include <QOpenGLFunctions>
#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>
#include <iostream>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_4_0_Core>

using namespace std;

MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    firstPaint(true),
    useGeometryShader(false),
    m_dataSource(0)
{
    m_effect = new CustomEffect();
}

void MultiBillboard::drawItem(QGLPainter *painter) {

    if(firstPaint) {
        if(effect()) {
            qDebug() << "Effect set from QML, should copy properties";
        }
        const QSurfaceFormat& format = painter->context()->format();
        qDebug() << "MultiBillboard: OpenGL version " << format.majorVersion() << "." << format.minorVersion();
        if ( ! (format.majorVersion() > 3 || (format.majorVersion() == 3 && format.minorVersion() >= 3)) )
        {
            qDebug("MultiBillboard: Geometry shader requires OpenGL >= 3.3. Falling back to CPU billboards.");
            useGeometryShader = false;
        } else {
            useGeometryShader = true;
        }
        firstPaint = false;
    }

    //    if(m_sortPoints == BackToFront) {
    //        QMultiMap<double, QVector3D> sortedPoints;
    //        for(int i = 0; i < m_points.count(); i++) {
    //            const QVector3D &center = m_points.at(i);
    //            const QVector4D &depthVector = painter->modelViewMatrix() * center;
    //            double depth = depthVector.z();
    //            sortedPoints.insert(depth, center);
    //        }
    //        m_points.clear();
    //        QMapIterator<double, QVector3D> i(sortedPoints);
    //        while(i.hasNext()) {
    //            m_points.push_back(i.next().value());
    //        }
    //        sortedPoints.clear();
    //    }

    double currentFps = 1000.0 / fpsTimer.restart();
    m_fps = 0.9*m_fps + 0.1 * currentFps;
    emit fpsChanged(m_fps);

    if(m_dataSource) {
        if(useGeometryShader) {
            drawGeometryShaderBillboards(painter);
        } else {
            drawCPUBillboards(painter);
        }
    }
}

void MultiBillboard::drawGeometryShaderBillboards(QGLPainter *painter) {
    const QArray<QVector3D> &positions = m_dataSource->getPositions();
    const QArray<QColor4ub> &colors = m_dataSource->getColors();
    const QArray<QVector2D> &sizes = m_dataSource->getSizes();

    QGLVertexBundle vertexBundle;
    vertexBundle.addAttribute(QGL::Position, positions);
    vertexBundle.addAttribute(QGL::Color, colors);
    vertexBundle.addAttribute(QGL::CustomVertex0, sizes);
    // TODO: Add sizes as an attribute so geometry shader knows how large the billboards should be

    painter->clearAttributes();

    // Setting our custom effect must be done before passing any data
    painter->setUserEffect(m_effect);

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

    // Set the rest of the vertex bundle (basically only positions)
    painter->setVertexBundle(vertexBundle);
    painter->draw(QGL::DrawingMode(QGL::Points), vertexBundle.vertexCount());
}

void MultiBillboard::drawCPUBillboards(QGLPainter *painter) {
    QGLVertexBundle vertexBundle;
    QGLIndexBuffer indexBuffer;

    vertices.clear();
    indices.clear();
    texCoords.clear();
    colorArray.clear();

    const QArray<QVector3D> &positions = m_dataSource->getPositions();
    const QArray<QColor4ub> &colors = m_dataSource->getColors();
    const QArray<QVector2D > &sizes = m_dataSource->getSizes();

    vertices.reserve(4*positions.count());
    texCoords.reserve(4*positions.count());

    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
    QVector3D right;
    right.setX(modelViewMatrix(0,0));
    right.setY(modelViewMatrix(0,1));
    right.setZ(modelViewMatrix(0,2));
    QVector3D up;
    up.setX(modelViewMatrix(1,0));
    up.setY(modelViewMatrix(1,1));
    up.setZ(modelViewMatrix(1,2));
    QVector3D a,b,c,d;

    QVector3D aOffset_x =  - right * 0.5;
    QVector3D aOffset_y =  - up * 0.5;

    QVector3D bOffset_x =  right * 0.5;
    QVector3D bOffset_y =  - up * 0.5;

    QVector3D cOffset_x =  right * 0.5;
    QVector3D cOffset_y =  up * 0.5;

    QVector3D dOffset_x =  - right * 0.5;
    QVector3D dOffset_y =  up * 0.5;
    QVector2D ta(0,0);
    QVector2D tb(0,1);
    QVector2D tc(1,1);
    QVector2D td(1,0);

    QVector3D normal(QVector3D::crossProduct(right, up));

    for(int i = 0; i <positions.count(); i++) {
        const QVector3D &center = positions.at(i);
        a = center + sizes.at(i).y()*aOffset_x + sizes.at(i).x()*aOffset_y;
        b = center + sizes.at(i).y()*bOffset_x + sizes.at(i).x()*bOffset_y;
        c = center + sizes.at(i).y()*cOffset_x + sizes.at(i).x()*cOffset_y;
        d = center + sizes.at(i).y()*dOffset_x + sizes.at(i).x()*dOffset_y;
        vertices.append(a, b, c, d);
        colorArray.append(colors.at(i), colors.at(i), colors.at(i), colors.at(i));
        texCoords.append(ta, tb, tc, td);
        indices.append(i*4 + 0, i*4 + 1, i*4 + 2);
        indices.append(i*4 + 2, i*4 + 3, i*4 + 0);
    }
    vertexBundle.addAttribute(QGL::Position, vertices);
    vertexBundle.addAttribute(QGL::TextureCoord0, texCoords);
    vertexBundle.addAttribute(QGL::Color, colorArray);
    indexBuffer.setIndexes(indices);

    painter->clearAttributes();
    // Set up normal attributes to use only one element
    painter->glDisableVertexAttribArray(GLuint(QGL::Normal));
    painter->glVertexAttrib3f(GLuint(QGL::Normal), normal.x(), normal.y(), normal.z());

    // Set the rest of the vertex bundle (basically only positions)
    painter->setVertexBundle(vertexBundle);
    painter->draw(QGL::DrawingMode(QGL::Triangles), indexBuffer, 0, indexBuffer.indexCount());
}

MultiBillboard::~MultiBillboard()
{
    delete m_effect;
}
