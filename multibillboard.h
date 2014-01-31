#ifndef MULTISPHERE2_H
#define MULTISPHERE2_H

#include "customeffect.h"

#include <QQuickItem3D>
#include <QGLAbstractScene>
#include <QElapsedTimer>

class MultiBillboard : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(SortMode sortPoints READ sortPoints WRITE setSortPoints NOTIFY sortPointsChanged)
    Q_PROPERTY(double fps READ fps NOTIFY fpsChanged)

public:
    explicit MultiBillboard(QQuickItem *parent = 0);
    ~MultiBillboard();

    SortMode sortPoints() const
    {
        return m_sortPoints;
    }

    void updatePoints();
    double fps() const
    {
        return m_fps;
    }

protected:
    void drawItem(QGLPainter *painter);
signals:

    void spacingChanged(double arg);

    void frequencyChanged(double arg);

    void sortPointsChanged(SortMode arg);

    void fpsChanged(double arg);

public slots:

    void setSortPoints(SortMode arg)
    {
        if (m_sortPoints != arg) {
            m_sortPoints = arg;
            emit sortPointsChanged(arg);
            updatePoints();
            update();
        }
    }

private:
    QElapsedTimer fpsTimer;
    QGLSceneNode *m_topNode;
    bool m_sceneSet;
    QGLSceneNode* m_geometry;
    QGLAbstractScene *scene;

    QArray<QVector3D> m_points;
    SortMode m_sortPoints;
    double m_fps;

    QArray<QVector3D> vertices;
    QArray<QVector3D> normals;
    QArray<QVector2D> texCoords;
    QArray<uint> indexes;

    CustomEffect* effect;
};

#endif // MULTISPHERE2_H
