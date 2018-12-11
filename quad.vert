#version 400 core

layout(location = 0) in vec3 position; //will also double as direction, since directional lights have no position
layout(location = 5) in vec2 inUV;

uniform vec3 lightPosition_world;
uniform mat4 v;

out vec2 uv;
out vec3 lightPosition_camera;

void main() {
    uv = inUV;
    lightPosition_camera = vec3(v * vec4(lightPosition_world, 1.0));
    gl_Position = vec4(position, 1.0);
}
