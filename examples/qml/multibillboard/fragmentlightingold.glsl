#version 330
in vec4 litColor;
in vec4 litSecondaryColor;
in vec4 qt_TexCoord0;
uniform sampler2D qt_Texture0;

void main(void)
{
    vec4 color = litColor * texture2D(qt_Texture0, qt_TexCoord0.st);
    gl_FragColor = clamp(color + litSecondaryColor, 0.0, 1.0);
}
