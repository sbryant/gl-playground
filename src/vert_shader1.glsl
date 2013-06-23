#version 150

in vec2 position;
in vec3 color;
in vec2 texCoords;

out vec3 Color;
out vec2 TexCoords;

uniform mat4 trans;

void main() {
	Color = color;
	TexCoords = texCoords;
	gl_Position = trans * vec4(position, 0.0, 1.0);
}
