#version 330
in vec4 qt_Vertex;
uniform mat4 qt_ModelViewProjectionMatrix;
out vec2 texCoord;
out vec4 someColor;
in vec2 qt_MultiTexCoord0;

void main(void)
{
    texCoord = qt_MultiTexCoord0;
    someColor = vec4(1.0, 0.0, 1.0, 1.0);
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
}
