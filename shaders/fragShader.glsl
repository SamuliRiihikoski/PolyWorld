#version 330 core
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;  

void main()
{
    color = vec4(0.5, 0.5, 0.5, 1.0);
} 