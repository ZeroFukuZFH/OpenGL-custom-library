#ifndef GLM_H
#define GLM_H

#include <math.h>

#define PI 3.14159265359

typedef struct Vector3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct Matrix4 {
    vec4 s1;
    vec4 s2;
    vec4 s3;
    vec4 s4;
} mat4;

// ---------- Constructors ----------

vec3 Vector3Constructor(float x, float y, float z);
vec4 Vector4Constructor(float x, float y, float z, float w);
mat4 MatrixConstructorDiagonal(float diagonal);
mat4 MaanytrixConstructor(vec4 s1, vec4 s2, vec4 s3, vec4 s4);

// ---------- Transformations ----------

mat4 translate(mat4 matrix, vec3 vector);
mat4 scale(mat4 matrix, vec3 vector);
mat4 rotate(mat4 matrix, float angle, vec3 vector);
mat4 perspective(float fov, float aspect,float near,float far);

// ---------- Utility ----------

mat4 ortho(
    float left,float right,
    float bottom,float top,
    float near,float far
);

float *valuePtr(mat4 matrix);
vec4 multiply(vec4 vector, mat4 matrix);
float radians(float degrees);

#endif
