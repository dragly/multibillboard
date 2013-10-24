#ifndef MULTISPHERE2_H
#define MULTISPHERE2_H

#include <QQuickItem3D>
#include <QGLAbstractScene>
#include <QElapsedTimer>

class MultiBillboard : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(SortMode sortPoints READ sortPoints WRITE setSortPoints NOTIFY sortPointsChanged)

public:
    explicit MultiBillboard(QQuickItem *parent = 0);
    ~MultiBillboard();

    SortMode sortPoints() const
    {
        return m_sortPoints;
    }

    void updatePoints();
protected:
    void drawItem(QGLPainter *painter);
signals:

    void spacingChanged(double arg);

    void frequencyChanged(double arg);

    void sortPointsChanged(SortMode arg);

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
    QGLSceneNode *m_topNode;
    bool m_sceneSet;
    QGLSceneNode* m_geometry;
    QGLAbstractScene *scene;

    QList<QVector3D> m_points;
    SortMode m_sortPoints;
};

#endif // MULTISPHERE2_H
