#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

out vec2 v_tex_coord;

uniform mat4 u_model_view_proj_matrix;

void main() {
  gl_Position = u_model_view_proj_matrix * position;
  v_tex_coord = tex_coord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_tex_coord;

uniform sampler2D u_tex;

void main() {
  vec4 tex_color = texture(u_tex, v_tex_coord);
  color = tex_color;
}