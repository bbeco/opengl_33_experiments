#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 color;

uniform vec3 offset;
void main(){
    gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0) + vec4(offset, 1.0);
    //color = vec4(aColor, 1.0f);
    color = gl_Position + vec4(0.5, 0.5, 0.0, 0.0);
}
