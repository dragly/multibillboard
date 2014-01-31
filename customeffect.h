#ifndef CUSTOMEFFECT_H
#define CUSTOMEFFECT_H

#include <QGLShaderProgramEffect>

class CustomEffect : public QGLShaderProgramEffect
{
public:
    CustomEffect();

    // QGLShaderProgramEffect interface
protected:
    bool beforeLink();
};

#endif // CUSTOMEFFECT_H
