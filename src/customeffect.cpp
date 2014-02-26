#include "customeffect.h"

#include <QOpenGLShaderProgram>

CustomEffect::CustomEffect() :
    QGLShaderProgramEffect(),
    m_useGeometryShader(false),
    m_size(QVector2D(1.0, 1.0)),
    m_color(QColor(255,255,255,255))
{
}

void CustomEffect::setUseGeometryShader(bool value)
{
    m_useGeometryShader = value;
}

bool CustomEffect::beforeLink() {

    QByteArray vertexCode = "#version 330\n"
            "uniform mat4 qt_ModelViewProjectionMatrix;\n"
            "in vec4 qt_Vertex;\n"
            "in vec2 qt_MultiTexCoord0;\n"
            "out vec2 texCoord;\n"
            "\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;\n"
            "    texCoord = qt_MultiTexCoord0;"
            "\n"
            "}\n"
            "\n";
    program()->addShaderFromSourceCode
            (QOpenGLShader::Vertex, vertexCode);
    //    setVertexShader(vertexCode);
    QByteArray appendix;
    if(m_useGeometryShader) {
        appendix = "Out";
    }

    QByteArray fragmentCode = "#version 330\n"
            "uniform vec4 color;\n"
            "out vec4 MyFragColor;\n"
            "in vec2 texCoord;\n"
            "uniform sampler2D qt_Texture0;\n"
            "\n"
            "void main(void) {\n"
            "    MyFragColor  = texture2D(qt_Texture0, texCoord.st);\n"
            "    MyFragColor  = MyFragColor * color;\n"
            "    if(MyFragColor.a < 0.9999) {\n"
            "        discard;\n"
            "    }\n"
            "}\n"
            "\n";
    program()->addShaderFromSourceCode
            (QOpenGLShader::Fragment, fragmentCode);
    //    qDebug() << fragmentCode;
    //    setFragmentShader(fragmentCode);
    if(m_useGeometryShader) {
        QByteArray geometryCode = "#version 330\n"
                "layout( points ) in;\n"
                "layout( triangle_strip, max_vertices = 4 ) out;\n"
                "uniform mat4 qt_ProjectionMatrix;\n"
                "uniform vec2 size;\n"
                "out vec2 texCoord;\n"
                "\n"
                "void main(void) {\n"
                "    vec4 pos = gl_in[0].gl_Position;\n"
                "    gl_Position = pos + qt_ProjectionMatrix*vec4(-size.x, -size.y, 0.0, 0.0);\n"
                "    texCoord = vec2(0.0, 0.0);\n"
                "    EmitVertex();\n"
                "    gl_Position = pos + qt_ProjectionMatrix*vec4(-size.x, size.y, 0.0, 0.0);\n"
                "    texCoord = vec2(0.0, 1.0);\n"
                "    EmitVertex();\n"
                "    gl_Position = pos + qt_ProjectionMatrix*vec4(size.x, -size.y, 0.0, 0.0);\n"
                "    texCoord = vec2(1.0, 0.0);\n"
                "    EmitVertex();\n"
                "    gl_Position = pos + qt_ProjectionMatrix*vec4(size.x, size.y, 0.0, 0.0);\n"
                "    texCoord = vec2(1.0, 1.0);\n"
                "    EmitVertex();\n"
                "    EndPrimitive();\n"
                "};\n"
                "\n";
        if(!program()->addShaderFromSourceCode(QOpenGLShader::Geometry,
                                               geometryCode)) {
            qCritical() << "Could not compile geometry shader! Log: \n"
                        << program()->log();
        }
    }
    return true;
}

void CustomEffect::afterLink() {
    m_sizeLocation = program()->uniformLocation("size");
    m_colorLocation = program()->uniformLocation("color");
}

void CustomEffect::setSize(const QVector2D& size) {
    m_size = size;
}

void CustomEffect::setColor(const QColor& color) {
    m_color = color;
}

void CustomEffect::update(QGLPainter *painter, QGLPainter::Updates updates) {
    QGLShaderProgramEffect::update(painter, updates);
    if(updates & QGLPainter::UpdateColor) {
        program()->setUniformValue(m_sizeLocation, m_size);
        program()->setUniformValue(m_colorLocation, m_color);
    }
}
