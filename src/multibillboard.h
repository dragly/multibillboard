#ifndef MULTISPHERE2_H
#define MULTISPHERE2_H

#include "customeffect.h"
#include "datasource.h"

#include <QQuickItem3D>
#include <QGLAbstractScene>
#include <QElapsedTimer>

class MultiBillboard : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(DataSource *dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(double fps READ fps NOTIFY fpsChanged)

public:
    explicit MultiBillboard(QQuickItem *parent = 0);
    virtual ~MultiBillboard();

    double fps() const
    {
        return m_fps;
    }

    DataSource *dataSource() const
    {
        return m_dataSource;
    }

protected:
    void drawItem(QGLPainter *painter);
signals:

    void spacingChanged(double arg);

    void frequencyChanged(double arg);

    void fpsChanged(double arg);

    void dataSourceChanged(DataSource *arg);

public slots:
    void setDataSource(DataSource *arg)
    {
        if (m_dataSource != arg) {
            m_dataSource = arg;
            emit dataSourceChanged(arg);
        }
    }

private:
    QElapsedTimer fpsTimer;
    QGLSceneNode *m_topNode;
    bool m_sceneSet;
    QGLSceneNode* m_geometry;
    QGLAbstractScene *scene;

    double m_fps;

    QArray<QVector3D> vertices;
    QArray<uint> indices;
    QArray<QVector3D> normals;
    QArray<QColor4ub> colorArray;
    QArray<QVector2D> texCoords;

    CustomEffect* m_effect;

    bool firstPaint;
    bool useGeometryShader;
    void drawCPUBillboards(QGLPainter *painter);
    void drawGeometryShaderBillboards(QGLPainter *painter);
    DataSource *m_dataSource;
};

#endif // MULTISPHERE2_H
