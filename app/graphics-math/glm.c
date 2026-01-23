#include "glm.h"
#include <stdlib.h>

vec3 Vector3Constructor(float x, float y, float z){
    return (vec3){x,y,z};
}

vec4 Vector4Constructor(float x, float y, float z, float w){
    return (vec4){x,y,z,w};
}

vec3 minus(vec3 a, vec3 b){
    return(vec3){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

vec3 add(vec3 a, vec3 b){
    return(vec3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

vec3 scaleV3f(vec3 v, float s){
    return (vec3){
        v.x * s,
        v.y * s,
        v.z * s
    };
}

vec3 scaleV3V3(vec3 a, vec3 b){
    return (vec3){
        a.x * b.z,
        a.y * b.z,
        a.z * b.z
    };
}

vec3 cross(vec3 a, vec3 b){
    vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

vec3 normalize(vec3 n){
    float magnitude = sqrt(
        pow((double)n.x,2) + 
        pow((double)n.y,2) + 
        pow((double)n.z,2)
    );

    return(vec3){
        n.x / magnitude,
        n.y / magnitude,
        n.z / magnitude
    };
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

mat4 lookAt(vec3 eye, vec3 center, vec3 up){
    vec3 f = normalize(vec3_sub(center, eye));  // forward
    vec3 s = normalize(cross(f, up));           // right
    vec3 u = cross(s, f);                       // corrected up

    mat4 result = {{0}};

    // Column-major order for OpenGL
    result.s1 = (vec4){ s.x,  u.x, -f.x, 0.0f };
    result.s2 = (vec4){ s.y,  u.y, -f.y, 0.0f };
    result.s3 = (vec4){ s.z,  u.z, -f.z, 0.0f };
    result.s4 = (vec4){ -dot(s, eye), -dot(u, eye), dot(f, eye), 1.0f };

    return result;
}

float dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

mat4 MatrixConstructorDiagonal(float diagonal){
    return (mat4){
        {diagonal,0,0,0},
        {0,diagonal,0,0},
        {0,0,diagonal,0},
        {0,0,0,diagonal}
    };
}

mat4 MatrixConstructor(vec4 s1,vec4 s2,vec4 s3,vec4 s4){
    return (mat4){s1,s2,s3,s4};
}

mat4 translate(mat4 matrix, vec3 vector){
    mat4 result = matrix;

    result.s4.x += matrix.s1.x * vector.x
                 + matrix.s2.x * vector.y
                 + matrix.s3.x * vector.z;

    result.s4.y += matrix.s1.y * vector.x
                 + matrix.s2.y * vector.y
                 + matrix.s3.y * vector.z;

    result.s4.z += matrix.s1.z * vector.x
                 + matrix.s2.z * vector.y
                 + matrix.s3.z * vector.z;

    result.s4.w += matrix.s1.w * vector.x
                 + matrix.s2.w * vector.y
                 + matrix.s3.w * vector.z;

    return result;
}

vec4 multiply(vec4 vector, mat4 matrix){
    vec4 result;

    result.x = matrix.s1.x * vector.x 
             + matrix.s2.x * vector.y 
             + matrix.s3.x * vector.z 
             + matrix.s4.x * vector.w;

    result.y = matrix.s1.y * vector.x 
             + matrix.s2.y * vector.y 
             + matrix.s3.y * vector.z 
             + matrix.s4.y * vector.w;

    result.z = matrix.s1.z * vector.x 
             + matrix.s2.z * vector.y 
             + matrix.s3.z * vector.z 
             + matrix.s4.z * vector.w;

    result.w = matrix.s1.w * vector.x 
             + matrix.s2.w * vector.y 
             + matrix.s3.w * vector.z 
             + matrix.s4.w * vector.w;

    return result;
}

float radians(float degrees){
    return degrees * PI/180;
}

mat4 scale(mat4 matrix, vec3 vector){
    mat4 result = matrix;

    result.s1.x *= vector.x;
    result.s1.y *= vector.x;
    result.s1.z *= vector.x;
    result.s1.w *= vector.x;

    result.s2.x *= vector.y;
    result.s2.y *= vector.y;
    result.s2.z *= vector.y;
    result.s2.w *= vector.y;

    result.s3.x *= vector.z;
    result.s3.y *= vector.z;
    result.s3.z *= vector.z;
    result.s3.w *= vector.z;

    return result;
}

static mat4 multiplyMat4(mat4 a, mat4 b){
    mat4 result;
    result.s1 = multiply(b.s1, a);
    result.s2 = multiply(b.s2, a);
    result.s3 = multiply(b.s3, a);
    result.s4 = multiply(b.s4, a);
    return result;
}

mat4 rotate(mat4 matrix, float angle, vec3 vector){
    float len = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    vec3 normal_vector = { vector.x / len , vector.y / len , vector.z / len };
    
    float x = normal_vector.x;
    float y = normal_vector.y; 
    float z = normal_vector.z;
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - c; 

    mat4 rotation;
    rotation.s1 = Vector4Constructor(t*x*x + c, t*x*y + z*s, t*x*z - y*s, 0);
    rotation.s2 = Vector4Constructor(t*x*y - z*s, t*y*y + c,   t*y*z + x*s, 0);
    rotation.s3 = Vector4Constructor(t*x*z + y*s, t*y*z - x*s, t*z*z + c, 0);
    rotation.s4 = Vector4Constructor(0, 0, 0, 1);

    return multiplyMat4(matrix, rotation);
}

float* valuePtr(mat4 matrix){
    float* arr = (float*)malloc(sizeof(float) * 16);
    if (!arr) return NULL;

    arr[0]  = matrix.s1.x; arr[1]  = matrix.s1.y; arr[2]  = matrix.s1.z; arr[3]  = matrix.s1.w;
    arr[4]  = matrix.s2.x; arr[5]  = matrix.s2.y; arr[6]  = matrix.s2.z; arr[7]  = matrix.s2.w;
    arr[8]  = matrix.s3.x; arr[9]  = matrix.s3.y; arr[10] = matrix.s3.z; arr[11] = matrix.s3.w;
    arr[12] = matrix.s4.x; arr[13] = matrix.s4.y; arr[14] = matrix.s4.z; arr[15] = matrix.s4.w;

    return arr;
}

mat4 perspective(float fov, float aspect,float near,float far){
    mat4 result = {{0}};

    float f = 1.0f / tanf(fov / 2.0f);
    
    // Column 1
    result.s1.x = f / aspect;
    result.s1.y = 0.0f;
    result.s1.z = 0.0f;
    result.s1.w = 0.0f;

    // Column 2
    result.s2.x = 0.0f;
    result.s2.y = f;
    result.s2.z = 0.0f;
    result.s2.w = 0.0f;

    // Column 3
    result.s3.x = 0.0f;
    result.s3.y = 0.0f;
    result.s3.z = -(far + near) / (far - near);
    result.s3.w = -1.0f;

    // Column 4
    result.s4.x = 0.0f;
    result.s4.y = 0.0f;
    result.s4.z = -(2.0f * far * near) / (far - near);
    result.s4.w = 0.0f;

    return result;
}

mat4 ortho(
    float left,float right,
    float bottom,float top,
    float near,float far
){
    mat4 matrix = {{0}};

    matrix.s1 = Vector4Constructor( 2/(right-left), 0, 0, -(right+left)/(right-left));
    matrix.s2 = Vector4Constructor( 0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom));
    matrix.s3 = Vector4Constructor( 0, 0, -2/(far-near), -(far+near)/(far-near));
    matrix.s4 = Vector4Constructor( 0,0,0,1 );

    return matrix;
}





