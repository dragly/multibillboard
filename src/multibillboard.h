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
    Q_PROPERTY(QUrl texture READ texture WRITE setTexture NOTIFY textureChanged)
    Q_PROPERTY(bool hasPeriodicCopies READ hasPeriodicCopies WRITE setHasPeriodicCopies NOTIFY hasPeriodicCopiesChanged)

public:
    explicit MultiBillboard(QQuickItem *parent = 0);
    virtual ~MultiBillboard();

    DataSource *dataSource() const
    {
        return m_dataSource;
    }

    bool bConnectedToOpenGLContextSignal;

    bool hasGeometryShaderSupport(QGLPainter *painter);
    void setTexture(const QUrl &value);
    QUrl texture() const
    {
        return m_texture;
    }

    bool hasPeriodicCopies() const
    {
        return m_effect->hasPeriodicCopies();
    }

protected:
    virtual void drawItem(QGLPainter *painter);
    virtual void drawEffectSetup(QGLPainter *painter, bool &viewportBlend, bool &effectBlend);
    virtual void drawEffectCleanup(QGLPainter *painter, bool &viewportBlend, bool &effectBlend);
signals:

    void spacingChanged(double arg);

    void frequencyChanged(double arg);

    void dataSourceChanged(DataSource *arg);

    void textureChanged(QUrl arg);

    void hasPeriodicCopiesChanged(bool arg);

public slots:
    void setDataSource(DataSource *arg)
    {
        if (m_dataSource != arg) {
            m_dataSource = arg;
            emit dataSourceChanged(arg);
        }
    }

    void setHasPeriodicCopies(bool arg)
    {
        if (m_effect->hasPeriodicCopies() != arg) {
            m_effect->setHasPeriodicCopies(arg);
            emit hasPeriodicCopiesChanged(arg);
        }
    }

private Q_SLOTS:
    void handleOpenglContextIsAboutToBeDestroyedYeah();

private:
    QGLSceneNode *m_topNode;
    bool m_sceneSet;
    QGLSceneNode* m_geometry;
    QGLAbstractScene *scene;

    QArray<QVector3D> vertices;
    QArray<uint> indices;
    QArray<QVector3D> normals;
    QArray<QColor4ub> colorArray;
    QArray<QVector2D> texCoords;
    bool m_firstVertexBuild;

    CustomEffect* m_effect;

    bool firstPaint;
    bool m_useGeometryShader;
    QQuickViewport* viewport;
    void drawCPUBillboards(QGLPainter *painter);
    void drawGeometryShaderBillboards(QGLPainter *painter);
    DataSource *m_dataSource;
    QUrl m_texture;
    QGLTexture2D *m_texture2D;
};

#endif // MULTISPHERE2_H
