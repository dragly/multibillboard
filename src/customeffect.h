#ifndef CUSTOMEFFECT_H
#define CUSTOMEFFECT_H

#include <QGLShaderProgramEffect>

class CustomEffect : public QGLShaderProgramEffect
{
public:
    CustomEffect();

    void setUseGeometryShader(bool value);
    void update(QGLPainter *painter, QGLPainter::Updates updates);
    void setSize(const QVector2D &size);
    void setColor(const QColor &color);
    QVector3D systemSize() const;
    void setSystemSize(const QVector3D &systemSize);

protected:
    bool beforeLink();

    void afterLink();
private:
    bool m_firstPaint;
    bool m_useGeometryShader;
    QOpenGLShaderProgram* m_program;
    bool m_regenerate;
    QVector2D m_size;
    QVector3D m_systemSize;
    QColor m_color;
    int m_sizeLocation;
    int m_colorLocation;
    int m_systemSizeLocation;
};

#endif // CUSTOMEFFECT_H
