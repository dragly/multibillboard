#ifndef CUSTOMEFFECT_H
#define CUSTOMEFFECT_H

#include <QGLShaderProgramEffect>

class CustomEffect : public QGLShaderProgramEffect
{
public:
    CustomEffect();

    void setUseGeometryShader(bool value);
protected:
    bool beforeLink();

private:
    bool m_firstPaint;
    bool m_useGeometryShader;
};

#endif // CUSTOMEFFECT_H
