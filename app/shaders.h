#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Shader {
    unsigned int ID;
    void(*use)(struct Shader *self);
    void(*delete)(struct Shader *self);
    void(*setBool)(struct Shader *self,const char* name, bool value);
    void(*setInt)(struct Shader *self,const char* name, int value);
    void(*setFloat)(struct Shader *self,const char* name, float value);
}Shader;

Shader shaderConstructor(const char* vertexPath, const char* fragmentPath);

#endif