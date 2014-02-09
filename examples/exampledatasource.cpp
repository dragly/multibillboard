#include "exampledatasource.h"
#include <QColor4ub>
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

ExampleDataSource::ExampleDataSource()
{

}

void ExampleDataSource::generateData() {
    m_colors.clear();
    m_sizes.clear();
    m_positions.clear();

    double spacing = 1;
    double frequency = 0.1;
    QColor4ub color(255,255,255,255);
    QSizeF size(1.0,1.0);

    for(int i = 0; i < m_numPointsPerDimension; i++) {
        for(int j = 0; j < m_numPointsPerDimension; j++) {
            for(int k = 0; k < m_numPointsPerDimension; k++) {
                QVector3D center(-m_numPointsPerDimension/2 + i * spacing,-m_numPointsPerDimension/2 + j,-m_numPointsPerDimension/2 + k);
                QVector3D center2 = center;
                center.setX(center.x() + cos(2 * frequency * center2.z()));
                center.setZ(center.z() + sin(2 * frequency * center2.y()));
                m_positions.push_back(center);
                m_colors.push_back(color);
                m_sizes.push_back(size);
            }
        }
    }
    cout << "Showing a total of " << m_positions.size() << " points." << endl;
}

const QArray<QVector3D> &ExampleDataSource::getPositions() {
    return m_positions;
}

const QArray<QColor4ub> &ExampleDataSource::getColors() {
    return m_colors;
}

const QArray<QSizeF> &ExampleDataSource::getSizes() {
    return m_sizes;
}
