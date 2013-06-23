#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef __GLPG_UTILS_H__
#define __GLPG_UTILS_H__

typedef float vec4[4];
typedef float vec3[3];
typedef vec4 mat4[4];

#define MAT4_IDENTITY           \
    {                           \
        { 1.0, 0.0, 0.0, 0.0 }, \
        { 0.0, 1.0, 0.0, 0.0 }, \
        { 0.0, 0.0, 1.0, 0.0 }, \
        { 0.0, 0.0, 0.0, 1.0 }  \
    }

#define VEC3_ROTATE_X { 1.0, 0.0, 0.0 }
#define VEC3_ROTATE_Y { 0.0, 1.0, 0.0 }
#define VEC3_ROTATE_Z { 0.0, 0.0, 1.0 }

#define MAT4_ROTATE_X(rads) { \
    { 1.0, 0.0, 0.0, 0.0 },              \
    { 0.0, cos(rads), sin(rads), 0.0 },  \
    { 0.0, -sin(rads), cos(rads), 0.0 }, \
    { 0.0, 0.0, 0.0, 1.0 }               \
}

#define MAT4_ROTATE_Y(rads) { \
    { cos(rads), 0.0, -sin(rads), 0.0 }, \
    { 0.0, 1.0, 0.0, 0.0 },              \
    { sin(rads), 0, cos(rads), 0.0 },    \
    { 0.0, 0.0, 0.0, 1.0 }               \
}

#define MAT4_ROTATE_Z(rads) { \
    { cos(rads), sin(rads), 0.0, 0.0 },  \
    { -sin(rads), cos(rads), 0.0, 0.0 }, \
    { 0.0, 0.0, 1.0, 0.0 },              \
    { 0.0, 0.0, 0.0, 1.0 }               \
}

#define GLPG_SHADER_READ_SIZE 1024

char *read_shader(const char* path);

void mat4_rotate(mat4 model, float angle, vec3 rotation_axis, mat4 result);
void mat4_mul(mat4 a, mat4 b, mat4 result);
#endif
