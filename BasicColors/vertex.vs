#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 norm;
out vec3 fragPos;

uniform mat4 transformation;
uniform mat4 model;

void main()
{
    gl_Position = transformation * vec4(aPos, 1.0f);
    norm = aNorm;
    fragPos = vec3(model*vec4(aPos, 1.0f));
}
