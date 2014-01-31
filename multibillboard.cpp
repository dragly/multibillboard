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
<<<<<<< HEAD
    int nPerDim = 50;
=======
    int nPerDim = 250;
>>>>>>> a896431892998f9206fffadd71583f070b3893d1
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
    indexes.clear();
    indexes.reserve(6*m_points.count());
    for(int i = 0; i < m_points.count(); i++) {
        indexes.append(i);
    }
}

void MultiBillboard::drawItem(QGLPainter *painter) {
    double currentFps = 1000.0 / fpsTimer.restart();
    m_fps = 0.9*m_fps + 0.1 * currentFps;
    emit fpsChanged(m_fps);

    QGLVertexBundle vertexBundle;
    QGLIndexBuffer indexBuffer;
    vertexBundle.addAttribute(QGL::Position, m_points);
    indexBuffer.setIndexes(indexes);

    painter->clearAttributes();

    // Set the rest of the vertex bundle (basically only positions)
    painter->setVertexBundle(vertexBundle);
    painter->setUserEffect(effect);
    painter->draw(QGL::DrawingMode(QGL::Points), indexBuffer, 0, indexBuffer.indexCount());
}

MultiBillboard::~MultiBillboard()
{
}
