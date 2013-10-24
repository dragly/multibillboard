#include "multibillboard.h"

#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>

MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    m_sortPoints(DefaultSorting)
{
    updatePoints();
}

void MultiBillboard::updatePoints() {
    m_points.clear();
    double spacing = 1;
    int nPerDim = 20;
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
}

void MultiBillboard::drawItem(QGLPainter *painter) {
    // Build the mesh
    QGLBuilder builder;
    builder.newSection(QGL::NoSmoothing);
    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
    QVector3D right;
    right.setX(modelViewMatrix(0,0));
    right.setY(modelViewMatrix(0,1));
    right.setZ(modelViewMatrix(0,2));
    QVector3D up;
    up.setX(modelViewMatrix(1,0));
    up.setY(modelViewMatrix(1,1));
    up.setZ(modelViewMatrix(1,2));
    QGeometryData quad;

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

    QVector3D a;
    QVector3D b;
    QVector3D c;
    QVector3D d;
    QVector2D ta(0,0);
    QVector2D tb(0,1);
    QVector2D tc(1,1);
    QVector2D td(1,0);
    for(int i = 0; i < m_points.length(); i++) {
        const QVector3D &center = m_points.at(i);
        if(painter->isCullable(center)) {
            continue;
        }
        double size = 0.2;
        a = center - right * (size * 0.5);
        b = center + right * size * 0.5;
        c = center + right * size * 0.5 + up * size;
        d = center - right * size * 0.5 + up * size;
        quad.appendVertex(a,b,c,d);
        quad.appendTexCoord(ta, tb, tc, td);
    }
    //    }
    builder.addQuads(quad);
    QGLSceneNode* geometry = builder.finalizedSceneNode();
    if(m_geometry) {
        delete m_geometry;
    }
    m_geometry = geometry;
    m_geometry->draw(painter);
}

MultiBillboard::~MultiBillboard()
{
}
