#version 150

in vec3 Color;
in vec2 TexCoords;

out vec4 outColor;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform float time;

void main() {
	vec4 sampleTex = texture(tex, TexCoords);
	vec4 sampleTex2 = texture(tex2, TexCoords);
	float factor = (sin(time * 3.0) + 1.0) / 2.0;

	outColor =  mix(sampleTex, sampleTex2, factor);
}
