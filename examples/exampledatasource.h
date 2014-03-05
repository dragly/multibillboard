#ifndef EXAMPLEDATASOURCE_H
#define EXAMPLEDATASOURCE_H
#include <compphys/multibillboard/datasource.h>
#include <QGLVertexBundle>

class ExampleDataSource : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(int numPointsPerDimension READ numPointsPerDimension WRITE setNumPointsPerDimension NOTIFY numPointsPerDimensionChanged)
public:
    ExampleDataSource();
    QArray<DataBundle*> *dataBundles();

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
private:
    int m_numPointsPerDimension;

    void generateData();
    QArray<DataBundle*> m_dataBundles;
};

#endif // EXAMPLEDATASOURCE_H
