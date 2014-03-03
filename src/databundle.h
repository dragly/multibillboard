#ifndef DATABUNDLE_H
#define DATABUNDLE_H

#include <QArray>
#include <QColor4ub>
#include <QVector2D>
#include <QVector3D>
#include <QGLVertexBundle>

class DataBundle
{
public:
    DataBundle();

    QArray<QVector3D> &positions();
    void setPositions(const QArray<QVector3D> &positions);

    const QColor& color() const;
    void setColor(const QColor &color);

    const QVector2D& size() const;
    void setSize(const QVector2D &size);

    const QGLVertexBundle& vertexBundle();
    void generateVertexBundle();

    QVector3D systemSize() const;
    void setSystemSize(const QVector3D &systemSize);

private:

    QArray<QVector3D> m_positions;
    QVector3D m_systemSize;
    QColor m_color;
    QVector2D m_size;
    QGLVertexBundle m_vertexBundle;

    bool m_vertexBundleDirty;
};

#endif // DATABUNDLE_H
