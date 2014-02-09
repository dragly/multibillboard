#include "customeffect.h"

#include <QOpenGLShaderProgram>

CustomEffect::CustomEffect() :
    QGLShaderProgramEffect()
{
    setVertexShaderFromFile("qml/multibillboard/vertexlighting.glsl");
    setFragmentShaderFromFile("qml/multibillboard/fragmentlighting.glsl");
}

bool CustomEffect::beforeLink() {
    if(!program()->addShaderFromSourceFile(QOpenGLShader::Geometry,
                                           "qml/multibillboard/geometrylighting.glsl")) {
        qCritical() << "Could not compile geometry shader! Log: \n"
                    << program()->log();
    }
    return true;
}
