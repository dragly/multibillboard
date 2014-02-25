#version 330
out vec4 MyFragColor;
in vec2 texCoord;
in vec4 litColorOut;
in vec4 litSecondaryColorOut;
uniform sampler2D qt_Texture0;

void main(void) {
    vec4 color = litColorOut * texture2D(qt_Texture0, texCoord.st);
    MyFragColor = clamp(color + litSecondaryColorOut, 0.0, 1.0);
    if(MyFragColor.a < 0.5) {
        discard;
    }
}

