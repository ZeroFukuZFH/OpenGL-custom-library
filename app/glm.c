#include "glm.h"
#include <math.h>
#define PI 3.14159265359

typedef struct Vector3 {
    float x;
    float y;
    float z;
}vec3;

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
}vec4;

typedef struct Matrix4{
    vec4 s1;
    vec4 s2;
    vec4 s3;
    vec4 s4;
}mat4;

vec3 Vector3Constructor(float x, float y, float z){
    vec3 vector = {x,y,z};
    return vector;
}

vec4 Vector4Constructor(float x, float y, float z, float w){
    vec4 vector = {x,y,z,w};
    return vector;
}

mat4 MatrixConstructorDiagonal(float diagonal){
    mat4 matrix;
    matrix.s1 = Vector4Constructor(diagonal,0,0,0);
    matrix.s2 = Vector4Constructor(0,diagonal,0,0);
    matrix.s3 = Vector4Constructor(0,0,diagonal,0);
    matrix.s4 = Vector4Constructor(0,0,0,diagonal);
    return matrix;
}

mat4 MatrixConstructor(vec4 s1,vec4 s2,vec4 s3,vec4 s4){
    mat4 m = {s1,s2,s3,s4};
    return m;
}

mat4 translate(mat4 matrix, vec3 vector){

}

float radians(float degrees){
    return degrees * PI/180;
}

mat4 scale(mat4 matrix, vec3 vector){
    
}

mat4 rotate(mat4 matrix, float angle, vec3 axis){
    
}



