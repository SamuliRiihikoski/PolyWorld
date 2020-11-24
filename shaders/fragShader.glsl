#version 330 core
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;  
uniform vec4 acolor;

void main()
{
    if(gl_FrontFacing)
        color = acolor;
    else
        color = vec4(0.0, 0.0, 0.0, 1.0);
} 