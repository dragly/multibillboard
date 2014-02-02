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
    m_sortPoints(DefaultSorting),
    firstPaint(true)
{
    effect = new CustomEffect();
    updatePoints();
}

void MultiBillboard::updatePoints() {
    m_points.clear();
    double spacing = 1;
    int nPerDim = 100;
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
    if(firstPaint) {
        const QSurfaceFormat& format = painter->context()->format();
        if ( ! (format.majorVersion() > 3 || (format.majorVersion() == 3 && format.minorVersion() >= 3)) )
        {
            qDebug() << "Only got version " << format.majorVersion() << "." << format.minorVersion();
            qFatal("MultiBillboard requires OpenGL >= 3.3");
            exit( 1 );
        }
        firstPaint = false;
    }

    double currentFps = 1000.0 / fpsTimer.restart();
    m_fps = 0.9*m_fps + 0.1 * currentFps;
    emit fpsChanged(m_fps);

    QGLVertexBundle vertexBundle;
    vertexBundle.addAttribute(QGL::Position, m_points);

    painter->clearAttributes();

    // Set the rest of the vertex bundle (basically only positions)
    painter->setVertexBundle(vertexBundle);
    painter->setUserEffect(effect);
    painter->draw(QGL::DrawingMode(QGL::Points), vertexBundle.vertexCount());
}

MultiBillboard::~MultiBillboard()
{
}
