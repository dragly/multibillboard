#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <QArray>
#include <QVector3D>
#include <QColor4ub>
#include <QObject>
#include <QSizeF>
#include <iostream>

using std::cerr;
using std::endl;

class DataSource : public QObject
{
    Q_OBJECT
private:
    QArray<QVector3D> m_tmp_positions;
    QArray<QColor4ub> m_tmp_colors;
    QArray<QSizeF> m_tmp_sizes;
public:

    virtual const QArray<QVector3D> &getPositions() { cerr << "Returning from abstract DataSource. This should not happen." << endl; return m_tmp_positions; }
    virtual const QArray<QColor4ub> &getColors() { cerr << "Returning from abstract DataSource. This should not happen." << endl; return m_tmp_colors; }
    virtual const QArray<QSizeF > &getSizes() { cerr << "Returning from abstract DataSource. This should not happen." << endl; return m_tmp_sizes; }

    DataSource();
};

#endif
