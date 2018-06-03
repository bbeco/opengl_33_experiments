#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    vec4 texel1 = texture(ourTexture1, texCoord);
    vec4 texel2 = texture(ourTexture2, texCoord);
    float blend = 0.2*texel2.a;
    FragColor = mix(texel1, texel2, blend);
}
