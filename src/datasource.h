#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <QArray>
#include <QVector3D>
#include <QColor4ub>
#include <QObject>
#include <QVector2D>
#include <QGLVertexBundle>
#include <iostream>

#include "databundle.h"

using std::cerr;
using std::endl;

class DataSource : public QObject
{
    Q_OBJECT
public:
    DataSource(QObject *parent = 0);

    virtual QArray<DataBundle*> *dataBundles() {
        cerr << "Returning from abstract DataSource."
                "This should not happen." << endl;
        return 0;
    }
};

#endif
