#version 400 core

in vec3 color;
in vec2 texc;
out vec4 fragColor;

uniform sampler2D tex;
uniform int useTexture = 0;
uniform int repeatBottomHalf = 0;

uniform int skybox = 0;

void main(){
    vec2 realTexc = texc;
    if (useTexture > 0 && repeatBottomHalf > 0) {
	if (texc.y > 1.0f) {
	    float fract = fract(texc.y);
	    if (fract < 0.5f) {
		realTexc.y += 0.5f;
	    }
	}
    }

    vec3 texColor = texture(tex, realTexc).bgr;
    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
    fragColor = vec4(color * texColor, 1);

    if (skybox > 0) {
	fragColor = vec4(texColor.rgb, 1);
    }
}
