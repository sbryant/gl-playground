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

    float rads = angle / 180.0 * M_PI;
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
