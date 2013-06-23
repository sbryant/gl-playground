#version 150

in vec2 position;
in vec3 color;
in vec2 texCoords;

out vec3 Color;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	Color = color;
	TexCoords = texCoords;
	gl_Position = proj * view * model * vec4(position, 0.0, 1.0);
}
