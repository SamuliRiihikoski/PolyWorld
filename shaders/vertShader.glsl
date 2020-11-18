#version 330 core
layout (location = 0) in vec3 position; // the position variable has attribute position 0

uniform mat4 model;     // model matrix
uniform mat4 view;      // view matrix   
uniform mat4 projection; // projection matrix

void main()
{
    gl_Position = projection * view  * model * vec4(position, 1.0f);
}