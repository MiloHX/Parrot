#type vertex
#version 330 core
layout(location = 0) in vec3  a_position;
layout(location = 1) in vec4  a_color;
layout(location = 2) in vec2  a_tex_coord;
layout(location = 3) in float a_tex_index;
layout(location = 4) in vec2  a_tex_scale;
    
uniform mat4 u_view_projection;

out vec4  v_color;
out vec2  v_tex_coord;
out float v_tex_index;
out vec2  v_tex_scale;

void main() {
    v_color     = a_color;
    v_tex_coord = a_tex_coord;
    v_tex_index = a_tex_index;
    v_tex_scale = a_tex_scale;

    gl_Position = u_view_projection * vec4(a_position, 1.0);
}


#type fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec4  v_color;
in vec2  v_tex_coord;
in float v_tex_index;
in vec2  v_tex_scale;

uniform sampler2D u_textures[32];


void main() {
    color = texture(u_textures[int(v_tex_index)], v_tex_coord * v_tex_scale) * v_color;
}