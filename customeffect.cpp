#include "customeffect.h"

#include <QOpenGLShaderProgram>

CustomEffect::CustomEffect() :
    QGLShaderProgramEffect()
{
    setVertexShader("#version 330\n"
                            "in vec4 qt_Vertex;"
                            "uniform mat4 qt_ModelViewProjectionMatrix;"
                            "out vec2 texCoord;"
                            "in vec2 qt_MultiTexCoord0;"
                            ""
                            "void main(void)"
                            "{"
                            "   texCoord = qt_MultiTexCoord0;"
                            "gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;"
                            "}");
    setFragmentShader("#version 330\n"
                              "out vec4 MyFragColor;"
                              "in vec2 texCoord;"
                              "uniform sampler2D qt_Texture0;"
                              ""
                              "void main(void) {"
                              "    MyFragColor = texture2D(qt_Texture0, texCoord.st);"
                              "}"
                              "");
}

bool CustomEffect::beforeLink() {
    QByteArray geometryString =
            "#version 330\n"
            "\n"
            "layout( points ) in;\n"
            "layout( triangle_strip, max_vertices = 4 ) out;\n"
            "uniform mat4 qt_ProjectionMatrix;"
            "out vec2 texCoord;"
            "\n"
            "void main(void) {\n"
            "       float scale = 0.1;"
            "       vec4 pos = gl_in[0].gl_Position;"
            "       gl_Position = pos + qt_ProjectionMatrix*vec4(-scale, -scale, 0.0, 0.0);\n"
            "       texCoord = vec2(0.0, 0.0);"
            "       EmitVertex();\n"
            "       gl_Position = pos + qt_ProjectionMatrix*vec4(-scale, scale, 0.0, 0.0);\n"
            "       texCoord = vec2(0.0, 1.0);"
            "       EmitVertex();\n"
            "       gl_Position = pos + qt_ProjectionMatrix*vec4(scale, -scale, 0.0, 0.0);\n"
            "       texCoord = vec2(1.0, 0.0);"
            "       EmitVertex();\n"
            "       gl_Position = pos + qt_ProjectionMatrix*vec4(scale, scale, 0.0, 0.0);\n"
            "       texCoord = vec2(1.0, 1.0);"
            "       EmitVertex();\n"
            "    EndPrimitive();\n"
            "}\n";
    if(!program()->addShaderFromSourceCode(QOpenGLShader::Geometry,
                                       geometryString)) {
        qCritical() << "Could not compile geometry shader! Log: \n"
                    << program()->log();
    }
    return true;
}
