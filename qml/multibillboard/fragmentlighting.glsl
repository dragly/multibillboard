#version 330
out vec4 MyFragColor;
in vec2 texCoord;
in vec4 someColorOut;
uniform sampler2D qt_Texture0;

void main(void) {
    MyFragColor = texture2D(qt_Texture0, texCoord.st);
}
