#include "customeffect.h"

#include <QOpenGLShaderProgram>

CustomEffect::CustomEffect() :
    QGLShaderProgramEffect(),
    m_useGeometryShader(false),
    m_hasPeriodicCopies(false),
    m_periodicCopiesAllowed(true),
    m_size(QVector2D(1.0, 1.0)),
    m_systemSize(QVector3D(0,0,0)),
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
            "uniform mat4 qt_ModelViewMatrix;\n"
            "in vec4 qt_Vertex;\n"
            "in vec2 qt_MultiTexCoord0;\n"
            "out vec2 texCoord;\n"
            "out vec4 vertexPosition;\n"
            "void main(void)\n"
            "{\n"
            "    vertexPosition = qt_Vertex;\n"
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
    if(m_useGeometryShader) {
        QByteArray geometryCode =
                (m_hasPeriodicCopies ?
                     QByteArray(
                         "#version 400\n"
                         "layout(invocations=27) in;\n"
                         )
                   :
                     QByteArray(
                         "#version 330\n"
                         )
                     )+
                "layout( points ) in;\n"
                "layout( triangle_strip, max_vertices = 4 ) out;\n"
                "uniform mat4 qt_ProjectionMatrix;\n"
                "uniform mat4 qt_ModelViewMatrix;\n"
                "uniform mat4 qt_ModelViewProjectionMatrix;\n"
                "uniform vec2 size;\n"
                "uniform vec3 systemSize;\n"
                "struct qt_SingleLightParameters {\n"
                "    vec4 position;\n"
                "    float linearAttenuation;\n"
                "    float constantAttenuation;\n"
                "    float quadraticAttenuation;\n"
                "};\n"
                "uniform qt_SingleLightParameters qt_Light;\n"
                "in vec4 vertexPosition[1];\n"
                "out vec2 texCoord;\n"
                "out float lightIntensity;\n"
                "\n"
                "void main(void) {\n"
                +(m_hasPeriodicCopies ?
                      QByteArray(
                          "    int x = gl_InvocationID % 3 - 1;\n"
                          "    int y = (gl_InvocationID/3) % 3-1;\n"
                          "    int z = (gl_InvocationID/9)-1;\n"
                          )
                    :
                      QByteArray(
                          "    int x = 0;\n"
                          "    int y = 0;\n"
                          "    int z = 0;\n"
                          )
                      )+
                "    vec4 displacement = vec4(systemSize.x*x,systemSize.y*y,systemSize.z*z,0);\n"
                "    vec4 vertex = qt_ModelViewMatrix * (vertexPosition[0] + displacement);\n"
                "    float distanceFromLight = length(qt_Light.position.xyz - vertex.xyz);"
                "    lightIntensity = clamp(1 / (qt_Light.linearAttenuation * distanceFromLight), 0.0, 1.0);\n"
                "    lightIntensity = lightIntensity * clamp(10 - qt_Light.constantAttenuation * distanceFromLight, 0.0, 1.0);\n"
                "    vec4 pos = gl_in[0].gl_Position + qt_ModelViewProjectionMatrix * displacement;\n"
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

    QByteArray fragmentCode = "#version 330\n"
            "uniform vec4 color;\n"
            "uniform sampler2D qt_Texture0;\n"
            "in float lightIntensity;\n"
            "in vec2 texCoord;\n"
            "out vec4 MyFragColor;\n"
            "\n"
            "void main(void) {\n"
            "    MyFragColor  = texture2D(qt_Texture0, texCoord.st);\n"
            "    MyFragColor  = MyFragColor * color;\n"
            "    MyFragColor = vec4(lightIntensity, lightIntensity, lightIntensity, 1.0) * MyFragColor;\n"
            "    if(MyFragColor.a < 0.9999) {\n"
            "        discard;\n"
            "    }\n"
            "}\n"
            "\n";
    program()->addShaderFromSourceCode
            (QOpenGLShader::Fragment, fragmentCode);
    //    qDebug() << fragmentCode;
    //    setFragmentShader(fragmentCode);
    return true;
}

void CustomEffect::afterLink() {
    m_sizeLocation = program()->uniformLocation("size");
    m_systemSizeLocation = program()->uniformLocation("systemSize");
    m_colorLocation = program()->uniformLocation("color");
}
bool CustomEffect::periodicCopiesAllowed() const
{
    return m_periodicCopiesAllowed;
}

void CustomEffect::setPeriodicCopiesAllowed(bool periodicCopiesAllowed)
{
    m_periodicCopiesAllowed = periodicCopiesAllowed;
}


bool CustomEffect::hasPeriodicCopies() const
{
    return m_hasPeriodicCopies;
}

void CustomEffect::setHasPeriodicCopies(bool hasPeriodicCopies)
{
    if(m_periodicCopiesAllowed) {
        m_hasPeriodicCopies = hasPeriodicCopies;
        setFragmentShader("");
    } else {
        m_hasPeriodicCopies = false;
    }
}

QVector3D CustomEffect::systemSize() const
{
    return m_systemSize;
}

void CustomEffect::setSystemSize(const QVector3D &systemSize)
{
    m_systemSize = systemSize;
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
        program()->setUniformValue(m_systemSizeLocation, m_systemSize);
        program()->setUniformValue(m_colorLocation, m_color);
    }
}
