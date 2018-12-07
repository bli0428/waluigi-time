#version 400 core

in vec3 color;
in vec2 texc;
out vec4 fragColor;

uniform sampler2D tex;
uniform int useTexture = 0;

void main(){
    vec3 texColor = texture(tex, texc).rgb;

    // just a test to see if texture mapping is working
    if (useTexture > 0) {
	fragColor = vec4(texColor, 1);
    } else {
	fragColor = vec4(color, 1);
    }

    //texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
    //fragColor = vec4(color * texColor, 1);
}
