#include "exampledatasource.h"
#include <QColor4ub>
#include <cmath>
#include <iostream>
#include <QSize>
#include <compphys/multibillboard/databundle.h>

using std::cout;
using std::endl;

ExampleDataSource::ExampleDataSource()
{

}

QArray<DataBundle *> *ExampleDataSource::dataBundles()
{
    return &m_dataBundles;
}

void ExampleDataSource::generateData() {

    double spacing = 1;
    double frequency = 0.1;
    QColor4ub color(255,255,255,255);
    QVector2D size(0.2,0.2);
//    QVector2D size(1.0,1.0);

    QArray<QVector3D> positions;
    QArray<QVector3D> positions2;
    for(int i = 0; i < m_numPointsPerDimension; i++) {
        for(int j = 0; j < m_numPointsPerDimension; j++) {
            for(int k = 0; k < m_numPointsPerDimension; k++) {
                QVector3D center(-m_numPointsPerDimension/2 + i * spacing,-m_numPointsPerDimension/2 + j*spacing,-m_numPointsPerDimension/2 + k*spacing);
                QVector3D center2 = center;
                center.setX(center.x() + cos(2 * frequency * center2.z()));
                center.setZ(center.z() + sin(2 * frequency * center2.y()));

                positions.push_back(center);

//                if(i % 4) {
//                    positions.push_back(center);
//                } else {
//                    positions2.push_back(center);
//                }
            }
        }
    }
    double systemSize = m_numPointsPerDimension*spacing;
    DataBundle *bundle = new DataBundle();
    bundle->setPositions(positions);
    bundle->setSystemSize(QVector3D(systemSize,systemSize,systemSize));
    bundle->setColor(QColor(0,255,178,255));
    bundle->setSize(QVector2D(0.1, 0.1));
    bundle->generateVertexBundle();
    m_dataBundles.append(bundle);
    DataBundle* bundle2 = new DataBundle();;
    bundle2->setPositions(positions2);
    bundle2->setColor(QColor(255,0,255,255));
    bundle2->setSize(QVector2D(0.2, 0.2));
    bundle2->generateVertexBundle();
    m_dataBundles.append(bundle2);
    cout << "Showing a total of " << positions.size() << " points." << endl;

}
