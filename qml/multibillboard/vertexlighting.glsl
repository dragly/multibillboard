#version 330
in vec4 qt_Vertex;
uniform mat4 qt_ModelViewProjectionMatrix;
out vec4 someColor;
in vec2 qt_MultiTexCoord0;
in vec3 qt_Normal;
uniform mat4 qt_ModelViewMatrix;
uniform mat3 qt_NormalMatrix;
uniform mat4 qt_ProjectionMatrix;

struct qt_MaterialParameters {
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
uniform qt_MaterialParameters qt_Material;

struct qt_SingleLightParameters {
    vec4 position;
    vec3 spotDirection;
    float spotExponent;
    float spotCutoff;
    float spotCosCutoff;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};
uniform qt_SingleLightParameters qt_Light;

out vec4 litColor;
out vec4 litSecondaryColor;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;

    // Calculate the vertex and normal to use for lighting calculations.
    vec4 vertex = qt_ModelViewMatrix * qt_Vertex;
    vec3 normal = normalize(qt_NormalMatrix * qt_Normal);

    // Start with the material's emissive color and the ambient scene color,
    // which have been combined into the emission parameter.
    vec4 color = qt_Material.emission;
    vec4 scolor = qt_Material.specular;

    // Viewer is at infinity.
    vec3 toEye = vec3(0, 0, 1);

    // Determine the angle between the normal and the light direction.
    vec4 pli = qt_Light.position;
    vec3 toLight;
    if (pli.w == 0.0)
        toLight = normalize(pli.xyz);
    else
        toLight = normalize(pli.xyz - vertex.xyz);
    float angle = max(dot(normal, toLight), 0.0);

    // Calculate the ambient and diffuse light components.
    vec4 adcomponent = qt_Material.ambient + angle * qt_Material.diffuse;

    // Calculate the specular light components.
    vec4 scomponent;
    if (angle != 0.0) {
        vec3 h = normalize(toLight + toEye);
        angle = max(dot(normal, h), 0.0);
        float srm = qt_Material.shininess;
        vec4 scm = qt_Material.specular;
        if (srm != 0.0)
            scomponent = pow(angle, srm) * scm;
        else
            scomponent = scm;
    } else {
        scomponent = vec4(0, 0, 0, 0);
    }

    // Apply the spotlight angle and exponent.
    if (qt_Light.spotCutoff != 180.0) {
        float spot = max(dot(normalize(vertex.xyz - pli.xyz),
                             qt_Light.spotDirection), 0.0);
        if (spot < qt_Light.spotCosCutoff) {
            adcomponent = vec4(0, 0, 0, 0);
            scomponent = vec4(0, 0, 0, 0);
        } else {
            spot = pow(spot, qt_Light.spotExponent);
            adcomponent *= spot;
            scomponent *= spot;
        }
    }

    // Apply attenuation to the colors.
    if (pli.w != 0.0) {
        float attenuation = qt_Light.constantAttenuation;
        float k1 = qt_Light.linearAttenuation;
        float k2 = qt_Light.quadraticAttenuation;
        if (k1 != 0.0 || k2 != 0.0) {
            float len = length(pli.xyz - vertex.xyz);
            attenuation += k1 * len + k2 * len * len;
        }
        color += adcomponent / attenuation;
        scolor += scomponent / attenuation;
    } else {
        color += adcomponent;
        scolor += scomponent;
    }

    // Generate the final output colors to pass to the fragment shader.
    float alpha = qt_Material.diffuse.a;
    litColor = vec4(clamp(color.rgb, 0.0, 1.0), alpha);
    litSecondaryColor = vec4(clamp(scolor.rgb, 0.0, 1.0), 0.0);
}
