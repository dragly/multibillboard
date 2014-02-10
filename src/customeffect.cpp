#include "customeffect.h"

#include <QOpenGLShaderProgram>

CustomEffect::CustomEffect() :
    QGLShaderProgramEffect()
{
//    setVertexShaderFromFile("qml/multibillboard/vertexlighting.glsl");
    QByteArray vertexCode = "#version 330\n"
            "in vec4 qt_Vertex;\n"
            "uniform mat4 qt_ModelViewProjectionMatrix;\n"
            "out vec4 someColor;\n"
            "in vec2 qt_MultiTexCoord0;\n"
            "in vec3 qt_Normal;\n"
            "uniform mat4 qt_ModelViewMatrix;\n"
            "uniform mat3 qt_NormalMatrix;\n"
            "uniform mat4 qt_ProjectionMatrix;\n"
            "\n"
            "struct qt_MaterialParameters {\n"
            "    vec4 emission;\n"
            "    vec4 ambient;\n"
            "    vec4 diffuse;\n"
            "    vec4 specular;\n"
            "    float shininess;\n"
            "};\n"
            "uniform qt_MaterialParameters qt_Material;\n"
            "\n"
            "struct qt_SingleLightParameters {\n"
            "    vec4 position;\n"
            "    vec3 spotDirection;\n"
            "    float spotExponent;\n"
            "    float spotCutoff;\n"
            "    float spotCosCutoff;\n"
            "    float constantAttenuation;\n"
            "    float linearAttenuation;\n"
            "    float quadraticAttenuation;\n"
            "};\n"
            "uniform qt_SingleLightParameters qt_Light;\n"
            "\n"
            "out vec4 litColor;\n"
            "out vec4 litSecondaryColor;\n"
            "\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;\n"
            "\n"
            "    // Calculate the vertex and normal to use for lighting calculations.\n"
            "    vec4 vertex = qt_ModelViewMatrix * qt_Vertex;\n"
            "    vec3 normal = normalize(qt_NormalMatrix * qt_Normal);\n"
            "\n"
            "    // Start with the material's emissive color and the ambient scene color,\n"
            "    // which have been combined into the emission parameter.\n"
            "    vec4 color = qt_Material.emission;\n"
            "    vec4 scolor = qt_Material.specular;\n"
            "\n"
            "    // Viewer is at infinity.\n"
            "    vec3 toEye = vec3(0, 0, 1);\n"
            "\n"
            "    // Determine the angle between the normal and the light direction.\n"
            "    vec4 pli = qt_Light.position;\n"
            "    vec3 toLight;\n"
            "    if (pli.w == 0.0)\n"
            "        toLight = normalize(pli.xyz);\n"
            "    else\n"
            "        toLight = normalize(pli.xyz - vertex.xyz);\n"
            "    float angle = max(dot(normal, toLight), 0.0);\n"
            "\n"
            "    // Calculate the ambient and diffuse light components.\n"
            "    vec4 adcomponent = qt_Material.ambient + angle * qt_Material.diffuse;\n"
            "\n"
            "    // Calculate the specular light components.\n"
            "    vec4 scomponent;\n"
            "    if (angle != 0.0) {\n"
            "        vec3 h = normalize(toLight + toEye);\n"
            "        angle = max(dot(normal, h), 0.0);\n"
            "        float srm = qt_Material.shininess;\n"
            "        vec4 scm = qt_Material.specular;\n"
            "        if (srm != 0.0)\n"
            "            scomponent = pow(angle, srm) * scm;\n"
            "        else\n"
            "            scomponent = scm;\n"
            "    } else {\n"
            "        scomponent = vec4(0, 0, 0, 0);\n"
            "    }\n"
            "\n"
            "    // Apply the spotlight angle and exponent.\n"
            "    if (qt_Light.spotCutoff != 180.0) {\n"
            "        float spot = max(dot(normalize(vertex.xyz - pli.xyz),\n"
            "                             qt_Light.spotDirection), 0.0);\n"
            "        if (spot < qt_Light.spotCosCutoff) {\n"
            "            adcomponent = vec4(0, 0, 0, 0);\n"
            "            scomponent = vec4(0, 0, 0, 0);\n"
            "        } else {\n"
            "            spot = pow(spot, qt_Light.spotExponent);\n"
            "            adcomponent *= spot;\n"
            "            scomponent *= spot;\n"
            "        }\n"
            "    }\n"
            "\n"
            "    // Apply attenuation to the colors.\n"
            "    if (pli.w != 0.0) {\n"
            "        float attenuation = qt_Light.constantAttenuation;\n"
            "        float k1 = qt_Light.linearAttenuation;\n"
            "        float k2 = qt_Light.quadraticAttenuation;\n"
            "        if (k1 != 0.0 || k2 != 0.0) {\n"
            "            float len = length(pli.xyz - vertex.xyz);\n"
            "            attenuation += k1 * len + k2 * len * len;\n"
            "        }\n"
            "        color += adcomponent / attenuation;\n"
            "        scolor += scomponent / attenuation;\n"
            "    } else {\n"
            "        color += adcomponent;\n"
            "        scolor += scomponent;\n"
            "    }\n"
            "\n"
            "    // Generate the final output colors to pass to the fragment shader.\n"
            "    float alpha = qt_Material.diffuse.a;\n"
            "    litColor = vec4(clamp(color.rgb, 0.0, 1.0), alpha);\n"
            "    litSecondaryColor = vec4(clamp(scolor.rgb, 0.0, 1.0), 0.0);\n"
            "}\n"
            "\n";
    setVertexShader(vertexCode);
//    setFragmentShaderFromFile("qml/multibillboard/fragmentlighting.glsl");
    QByteArray fragmentCode = "#version 330\n"
            "out vec4 MyFragColor;\n"
            "in vec2 texCoord;\n"
            "in vec4 litColorOut;\n"
            "in vec4 litSecondaryColorOut;\n"
            "uniform sampler2D qt_Texture0;\n"
            "\n"
            "void main(void) {\n"
            "    vec4 color = litColorOut * texture2D(qt_Texture0, texCoord.st);\n"
            "    MyFragColor = clamp(color + litSecondaryColorOut, 0.0, 1.0);\n"
            "}\n"
            "\n";
    setFragmentShader(fragmentCode);
}

bool CustomEffect::beforeLink() {
    QByteArray geometryCode = "#version 330\n"
            "layout( points ) in;\n"
            "layout( triangle_strip, max_vertices = 4 ) out;\n"
            "uniform mat4 qt_ProjectionMatrix;\n"
            "in vec4 litColor[1];\n"
            "in vec4 litSecondaryColor[1];\n"
            "out vec4 litColorOut;\n"
            "out vec4 litSecondaryColorOut;\n"
            "out vec2 texCoord;\n"
            "\n"
            "void main(void) {\n"
            "    float scale = 0.1;\n"
            "    vec4 pos = gl_in[0].gl_Position;\n"
            "    gl_Position = pos + qt_ProjectionMatrix*vec4(-scale, -scale, 0.0, 0.0);\n"
            "    litColorOut = litColor[0];\n"
            "    litSecondaryColorOut = litSecondaryColor[0];\n"
            "    texCoord = vec2(0.0, 0.0);\n"
            "    EmitVertex();\n"
            "    gl_Position = pos + qt_ProjectionMatrix*vec4(-scale, scale, 0.0, 0.0);\n"
            "    litColorOut = litColor[0];\n"
            "    litSecondaryColorOut = litSecondaryColor[0];\n"
            "    texCoord = vec2(0.0, 1.0);\n"
            "    EmitVertex();\n"
            "    gl_Position = pos + qt_ProjectionMatrix*vec4(scale, -scale, 0.0, 0.0);\n"
            "    litColorOut = litColor[0];\n"
            "    litSecondaryColorOut = litSecondaryColor[0];\n"
            "    texCoord = vec2(1.0, 0.0);\n"
            "    EmitVertex();\n"
            "    gl_Position = pos + qt_ProjectionMatrix*vec4(scale, scale, 0.0, 0.0);\n"
            "    litColorOut = litColor[0];\n"
            "    litSecondaryColorOut = litSecondaryColor[0];\n"
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
    return true;
}
