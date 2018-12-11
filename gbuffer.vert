#version 400 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 5) in vec2 texCoord; // UV texture coordinates

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

void main(void)
{
    TexCoords = texCoord;

    vec4 position_cameraSpace = v * m * vec4(position, 1.0);
    vec4 normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);

    FragPos = position_cameraSpace;
    Normal = normal_cameraSpace;

    gl_Position = p * position_cameraSpace;
}
