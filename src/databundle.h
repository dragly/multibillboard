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

    const QArray<QVector3D>& positions() const;
    void setPositions(const QArray<QVector3D> &positions);

    const QColor& color() const;
    void setColor(const QColor &color);

    const QVector2D& size() const;
    void setSize(const QVector2D &size);

    const QGLVertexBundle& vertexBundle();
    void generateVertexBundle();

private:

    QArray<QVector3D> m_positions;
    QColor m_color;
    QVector2D m_size;
    QGLVertexBundle m_vertexBundle;

    bool m_vertexBundleDirty;
};

#endif // DATABUNDLE_H
