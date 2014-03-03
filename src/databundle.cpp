#include "databundle.h"

DataBundle::DataBundle() :
    m_vertexBundleDirty(false)
{
}
QArray<QVector3D> &DataBundle::positions()
{
    return m_positions;
}

void DataBundle::generateVertexBundle() {
    m_vertexBundle = QGLVertexBundle();
    m_vertexBundle.addAttribute(QGL::Position, m_positions);
    m_vertexBundleDirty = false;
}
QVector3D DataBundle::systemSize() const
{
    return m_systemSize;
}

void DataBundle::setSystemSize(const QVector3D &systemSize)
{
    m_systemSize = systemSize;
}


void DataBundle::setPositions(const QArray<QVector3D> &positions)
{
    m_positions = positions;
    m_vertexBundleDirty = true;
}
const QColor &DataBundle::color() const
{
    return m_color;
}

void DataBundle::setColor(const QColor &color)
{
    m_color = color;
}
const QVector2D &DataBundle::size() const
{
    return m_size;
}

void DataBundle::setSize(const QVector2D &size)
{
    m_size = size;
}

const QGLVertexBundle &DataBundle::vertexBundle()
{
    if(m_vertexBundleDirty) {
        generateVertexBundle();
    }
    return m_vertexBundle;
}



