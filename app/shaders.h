#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "glm.h"
typedef struct Shader {
    unsigned int ID;
    void(*use)(struct Shader *self);
    void(*delete)(struct Shader *self);
    void(*setBool)(struct Shader *self,const char* name, bool value);
    void(*setInt)(struct Shader *self,const char* name, int value);
    void(*setFloat)(struct Shader *self,const char* name, float value);
    void(*setMat4)(struct Shader *self, const char* name,mat4 matrix);
}Shader;

Shader shaderConstructor(const char* vertexPath, const char* fragmentPath);

#endif