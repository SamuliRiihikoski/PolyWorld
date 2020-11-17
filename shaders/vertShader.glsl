#version 330 core
layout (location = 0) in vec3 position; // the position variable has attribute position 0

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main()
{
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}