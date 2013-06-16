#include <stddef.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define GLPG_SHADER_READ_SIZE 1024

char *read_shader(const char* path) {
	int fd = open(path, O_RDONLY);
	char buffer[GLPG_SHADER_READ_SIZE];
	char *result = NULL;
	size_t file_size = 0;
	size_t bytes_read = 0;
	size_t offset = 0;

	while((bytes_read = read(fd, &buffer, GLPG_SHADER_READ_SIZE))) {
		if(bytes_read == -1)
			return NULL;

		offset = file_size;
		file_size += bytes_read;
		result = (char*)realloc(result, file_size + 1);
		memcpy(result + offset, &buffer, bytes_read);
		result[file_size] = '\0';
	}

	close(fd);

	return result;
};

int main(int argc, char** argv) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		goto error;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Turn on modern extension checking before initializing GLEW */
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		/* Problem: glewInit faialed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		goto error;

	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	float verts[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	/* Create a simple VAO for drawing */
	GLuint vao;
	glGenVertexArrays(1, &vao);

	/* This tells GL which vao to operate against */
	glBindVertexArray(vao);

	/* Generate a VBO handler to associate data */
	GLuint vbo;
	glGenBuffers(1, &vbo);

	/* Activate the buffer */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	/* Upload the data to the GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	/* load the shaders */
	char *vert_shader_source = read_shader("src/vert_shader1.glsl");
	assert(vert_shader_source != NULL);
	char *frag_shader_source = read_shader("src/frag_shader1.glsl");
	assert(frag_shader_source != NULL);

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, (const GLchar **)&vert_shader_source, NULL);

	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, (const GLchar **)&frag_shader_source, NULL);

	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	/* handler for binding shaders */
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);

	glBindFragDataLocation(shader_program, 0, "outColor");

	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	free(vert_shader_source);
	free(frag_shader_source);

	/* Tell GL how our data is laid out */
	GLint pos_attrib = glGetAttribLocation(shader_program, "position");
	glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(pos_attrib);
	GLint vert_color = glGetUniformLocation(shader_program, "inColor");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		glUniform3f(vert_color, 1.0f, 0.0f, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* clean up */
	glDeleteProgram(shader_program);
	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
	return 0;
  error:
	glfwTerminate();
	return -1;
}
