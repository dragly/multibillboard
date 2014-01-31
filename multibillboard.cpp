#include "multibillboard.h"

#include <QOpenGLFunctions>
#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>
#include <iostream>

using namespace std;

MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    m_sortPoints(DefaultSorting)
{
    effect = new CustomEffect();
    effect->setVertexShader("#version 330 core\n"
                            "in vec4 qt_Vertex;"
                            "uniform mat4 qt_ModelViewProjectionMatrix;"
                            "out vec2 texCoord;"
                            "in vec2 qt_MultiTexCoord0;"
                            ""
                            "void main(void)"
                            "{"
                            "   texCoord = qt_MultiTexCoord0;"
                                "gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;"
                            "}");
    effect->setFragmentShader("#version 330 core\n"
                              "out vec4 MyFragColor;"
                              "in vec2 texCoord;"
                              "uniform sampler2D qt_Texture0;"
                              ""
                              "void main(void) {"
                              "    MyFragColor = texture2D(qt_Texture0, texCoord.st);"
//                              "    MyFragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                              "}"
                              "");
    updatePoints();
}

void MultiBillboard::updatePoints() {
    m_points.clear();
    double spacing = 1;
    int nPerDim = 90;
    double frequency = 0.1;
    for(int i = 0; i < nPerDim; i++) {
        for(int j = 0; j < nPerDim; j++) {
            for(int k = 0; k < nPerDim; k++) {
                QVector3D center(-nPerDim/2 + i * spacing,-nPerDim/2 + j,-nPerDim/2 + k);
                QVector3D center2 = center;
                center.setX(center.x() + cos(2 * frequency * center2.z()));
                center.setZ(center.z() + sin(2 * frequency * center2.y()));
                m_points.push_back(center);
            }
        }
    }
    cout << "Showing a total of " << m_points.size() << " points." << endl;
}

void MultiBillboard::drawItem(QGLPainter *painter) {
    double currentFps = 1000.0 / fpsTimer.restart();
    m_fps = 0.9*m_fps + 0.1 * currentFps;
    emit fpsChanged(m_fps);
    // Build the mesh
//    QGLBuilder builder;
//    builder.newSection(QGL::NoSmoothing);

    QGLVertexBundle vertexBundle;
    QGLIndexBuffer indexBuffer;

    vertices.clear();
//    normals.clear();
    texCoords.clear();
    indexes.clear();

    vertices.reserve(4*m_points.length());
//    normals.reserve(4*m_points.length());
    texCoords.reserve(4*m_points.length());
    indexes.reserve(6*m_points.length());

    if(m_sortPoints == BackToFront) {
        QMultiMap<double, QVector3D> sortedPoints;
        for(int i = 0; i < m_points.length(); i++) {
            const QVector3D &center = m_points.at(i);
            const QVector4D &depthVector = painter->modelViewMatrix() * center;
            double depth = depthVector.z();
            sortedPoints.insert(depth, center);
        }
        m_points.clear();
        QMapIterator<double, QVector3D> i(sortedPoints);
        while(i.hasNext()) {
            m_points.push_back(i.next().value());
        }
        sortedPoints.clear();
    }

    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
    QVector3D right;
    right.setX(modelViewMatrix(0,0));
    right.setY(modelViewMatrix(0,1));
    right.setZ(modelViewMatrix(0,2));
    QVector3D up;
    up.setX(modelViewMatrix(1,0));
    up.setY(modelViewMatrix(1,1));
    up.setZ(modelViewMatrix(1,2));
    QVector3D a;
    QVector3D b;
    QVector3D c;
    QVector3D d;
    QVector3D aOffset =  - right * 0.5 - up * 0.5;
    QVector3D bOffset =  right * 0.5 - up * 0.5;
    QVector3D cOffset =  right * 0.5 + up * 0.5;
    QVector3D dOffset =  - right * 0.5 + up * 0.5;
    QVector2D ta(0,0);
    QVector2D tb(0,1);
    QVector2D tc(1,1);
    QVector2D td(1,0);
    QVector3D normal(QVector3D::crossProduct(right, up));
    for(int i = 0; i < m_points.length(); i++) {
        const QVector3D &center = m_points.at(i);
//        if(painter->isCullable(center)) {
//            continue;
//        }
        double size = 0.2;
        a = center + size * aOffset;
        b = center + size * bOffset;
        c = center + size * cOffset;
        d = center + size * dOffset;
        vertices.append(a, b, c, d);
//        vertices.append(center);
        texCoords.append(ta, tb, tc, td);
//        normals.append(normal, normal, normal, normal);
//        indexes.append(i);
        indexes.append(i*4 + 0, i*4 + 1, i*4 + 2);
        indexes.append(i*4 + 2, i*4 + 3, i*4 + 0);
    }
    vertexBundle.addAttribute(QGL::Position, vertices);
    vertexBundle.addAttribute(QGL::TextureCoord0, texCoords);
//    vertexBundle.addAttribute(QGL::Normal, normals);
    indexBuffer.setIndexes(indexes);

    painter->clearAttributes();
    // Set up normal attributes to use only one element
//    painter->glDisableVertexAttribArray(GLuint(QGL::Normal));
//    painter->glVertexAttrib3f(GLuint(QGL::Normal), normal.x(), normal.y(), normal.z());

    // Set the rest of the vertex bundle (basically only positions)
    painter->setVertexBundle(vertexBundle);
    painter->setUserEffect(effect);
    painter->draw(QGL::DrawingMode(QGL::Triangles), indexBuffer, 0, indexBuffer.indexCount());
}

MultiBillboard::~MultiBillboard()
{
}
