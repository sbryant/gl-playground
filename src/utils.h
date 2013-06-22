#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#ifndef __GLPG_UTILS_H__
#define __GLPG_UTILS_H__

#define GLPG_SHADER_READ_SIZE 1024

char *read_shader(const char* path);
#endif
