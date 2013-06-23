#include "utils.h"

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

void mat4_rotate(mat4 model, float angle, vec3 axis, mat4 result) {

    float rads = (angle * M_PI) / 180.0;
    mat4 rotate_mat;

    if(axis[0] == 1.0) {
        mat4 temp = MAT4_ROTATE_X(rads);
        memcpy(rotate_mat, temp, sizeof(mat4));
    }

    else if(axis[1] == 1.0) {
        mat4 temp = MAT4_ROTATE_Y(rads);
        memcpy(rotate_mat, temp, sizeof(mat4));
    }
    else if(axis[2] == 1.0) {
        mat4 temp = MAT4_ROTATE_Z(rads);
        memcpy(rotate_mat, temp, sizeof(mat4));
    }

    mat4_mul(model, rotate_mat, result);
}

void mat4_lookat(vec3 eye, vec3 center, vec3 up, mat4 result) {

    vec3 z_axis; vec3 x_axis; vec3 y_axis;
    vec3 eye_sub_center; vec3 up_cross_z;
    vec3_sub(eye, center, eye_sub_center);
    vec3_normalize(eye_sub_center, z_axis);
    vec3_cross(up, z_axis, up_cross_z);
    vec3_normalize(up_cross_z, x_axis);
    vec3_cross(z_axis, x_axis, y_axis);

    result[0][0] = x_axis[0];
    result[1][0] = x_axis[1];
    result[2][0] = x_axis[2];

    result[0][1] = y_axis[0];
    result[1][1] = y_axis[1];
    result[2][1] = y_axis[2];

    result[0][2] = z_axis[0];
    result[1][2] = z_axis[1];
    result[2][2] = z_axis[2];

    result[3][0] = vec3_dot(x_axis, eye);
    result[3][1] = vec3_dot(y_axis, eye);
    result[3][2] = vec3_dot(z_axis, eye);
    result[3][3] = 1.0;
}

void vec3_sub(vec3 a, vec3 b, vec3 res) {
    res[0] = a[0] - b[0];
    res[1] = a[1] - b[1];
    res[3] = a[2] - b[2];
}

void vec3_normalize(vec3 v, vec3 result) {
    float mag = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));

    result[0] = v[0] / mag;
    result[1] = v[1] / mag;
    result[2] = v[2] / mag;
}

void vec3_cross(vec3 a, vec3 b, vec3 result) {
    result[0] = (a[1] * b[2]) - (a[2] * b[1]);
    result[1] = (a[2] * b[0]) - (a[0] * b[2]);
    result[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

float vec3_dot(vec3 a, vec3 b) {
    return (a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]);
}

void mat4_perspective(float fovy, float w, float h, float z_near, float z_far, mat4 result) {
    float aspect = w / h;
    float depth = z_far - z_near;
    float y_scale = 1.0 / tan(fovy * M_PI / 360.0);
    float x_scale = y_scale / aspect;

    result[0][0] = x_scale;
    result[1][1] = y_scale;
    result[2][2] = (z_far + z_near) / depth;
    result[2][3] = (2 * z_far * z_near) / depth;
    result[3][2] = -1.0;
}

void mat4_mul(mat4 a, mat4 b, mat4 result) {
    /* go through the rows multiplying the rows of by the columns of be and adding the sums */
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            float temp = 0.0;
            for(int k = 0; k < 4; k++) {
                temp += a[i][k] * b[k][j];
            }
            result[i][j] = temp;
        }
    }
}