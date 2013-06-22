#include <stddef.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

#include "utils.h"

#define GLPG_VERTEX_STRIDE sizeof(float) * 7

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
	window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
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

	SDL_Init(SDL_INIT_VIDEO);
	/* SDL Image setup */
	int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int img_flags = IMG_Init(img_init_flags);
	if((img_flags & img_init_flags) != img_init_flags) {
		fprintf(stderr, "Error: Seting up SDL_image: %s\n", IMG_GetError());
		goto error;
	}

	SDL_Surface* sample_image = IMG_Load("sample.png");
	if(!sample_image) {
		fprintf(stderr, "Error: Loading image: %s\n", IMG_GetError());
		goto error;
	}

	SDL_Surface* sample_image2 = IMG_Load("sample2.png");
	if(!sample_image2) {
		fprintf(stderr, "Error: Loading image: %s\n", IMG_GetError());
		goto error;
	}

	float verts[] = {
		/* x, y,      R,G,B             S,T*/
		-0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
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

	GLuint ebo;
	glGenBuffers(1, &ebo);

    /* Oreder in which to draw rect */
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLuint texture[2];
	glGenTextures(2, texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_LockSurface(sample_image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sample_image->w, sample_image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sample_image->pixels);
	SDL_UnlockSurface(sample_image);
	SDL_FreeSurface(sample_image);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_LockSurface(sample_image2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sample_image2->w, sample_image2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sample_image2->pixels);
	SDL_UnlockSurface(sample_image2);
	SDL_FreeSurface(sample_image2);

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
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, GLPG_VERTEX_STRIDE, 0);

	GLint color_attrib = glGetAttribLocation(shader_program, "color");
	glEnableVertexAttribArray(color_attrib);
	glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, GLPG_VERTEX_STRIDE, (void*)(sizeof(GLfloat) * 2));

	GLint tex_attrib = glGetAttribLocation(shader_program, "texCoords");
	glEnableVertexAttribArray(tex_attrib);
	glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, GLPG_VERTEX_STRIDE, (void*)(sizeof(GLfloat) * 5));

    glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "tex2"), 1);

    GLint uniform_time = glGetUniformLocation(shader_program, "time");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		float factor = (float)glfwGetTime() / 3600.0;
		glUniform1i(uniform_time, factor);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	IMG_Quit();
	return 0;
  error:
	glfwTerminate();
	return -1;
}
