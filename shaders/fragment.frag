#version 460 core

uniform sampler2D u_texture;

in vec2 v_texcoord;

layout(location = 0) out vec4 frag_color;

void main() {
    frag_color = texture(u_texture, v_texcoord);
}
