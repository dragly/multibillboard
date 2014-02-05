#version 330
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;
uniform mat4 qt_ProjectionMatrix;
in vec4 someColor[1];
out vec4 someColorOut;
out vec2 texCoord;

void main(void) {
    float scale = 0.1;
    vec4 pos = gl_in[0].gl_Position;
    gl_Position = pos + qt_ProjectionMatrix*vec4(-scale, -scale, 0.0, 0.0);
    someColorOut = someColor[0];
    texCoord = vec2(0.0, 0.0);
    EmitVertex();
    gl_Position = pos + qt_ProjectionMatrix*vec4(-scale, scale, 0.0, 0.0);
    someColorOut = someColor[0];
    texCoord = vec2(0.0, 1.0);
    EmitVertex();
    gl_Position = pos + qt_ProjectionMatrix*vec4(scale, -scale, 0.0, 0.0);
    someColorOut = someColor[0];
    texCoord = vec2(1.0, 0.0);
    EmitVertex();
    gl_Position = pos + qt_ProjectionMatrix*vec4(scale, scale, 0.0, 0.0);
    someColorOut = someColor[0];
    texCoord = vec2(1.0, 1.0);
    EmitVertex();
    EndPrimitive();
};
