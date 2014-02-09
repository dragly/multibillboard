#ifndef EXAMPLEDATASOURCE_H
#define EXAMPLEDATASOURCE_H
#include <compphys/multibillboard/datasource.h>

class ExampleDataSource : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(int numPointsPerDimension READ numPointsPerDimension WRITE setNumPointsPerDimension NOTIFY numPointsPerDimensionChanged)
private:
    QArray<QVector3D> m_positions;
    QArray<QColor4ub> m_colors;
    QArray<QSizeF > m_sizes;
    int m_numPointsPerDimension;
    void generateData();
public:
    ExampleDataSource();
    virtual const QArray<QVector3D> &getPositions();
    virtual const QArray<QColor4ub> &getColors();
    virtual const QArray<QSizeF > &getSizes();

    int numPointsPerDimension() const
    {
        return m_numPointsPerDimension;
    }
public slots:
    void setNumPointsPerDimension(int arg)
    {
        if (m_numPointsPerDimension != arg) {
            m_numPointsPerDimension = arg;
            emit numPointsPerDimensionChanged(arg);
            generateData();
        }
    }
signals:
    void numPointsPerDimensionChanged(int arg);
};

#endif // EXAMPLEDATASOURCE_H
