#version 400
in vec2 uv;
out vec4 color;

uniform sampler2D sampler2d;


void main(){
    color = texture(sampler2d, uv);
}
