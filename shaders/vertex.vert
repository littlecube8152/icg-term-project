#version 460 core

layout(location = 0) in vec2 a_position;
in vec2 a_texcoord;

out vec2 v_texcoord;

void main() {
    v_texcoord = a_texcoord;
    gl_Position = vec4(a_position.xy, 0.0f, 1.0f);
}
