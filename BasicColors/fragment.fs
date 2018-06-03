#version 330 core
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambientStrength;
uniform vec3 cameraPos;
uniform float specularStrength;

void main()
{

    float ambient = ambientStrength;

    vec3 lightDirection = normalize(lightPosition - fragPos);
    float diffuse = max(dot(lightDirection, norm), 0);

    vec3 viewDirection = normalize(cameraPos - fragPos);
    vec3 reflection = reflect(-lightDirection, norm);
    float specular = specularStrength*pow(max(dot(reflection, viewDirection), 0.0f), 32);

    fragColor = vec4((ambient + diffuse + specular)*lightColor*objectColor, 1.0f);
}
