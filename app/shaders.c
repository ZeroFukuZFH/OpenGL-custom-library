#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shaders.h"
#include "glm.h"

// checker methods
static void checkShaderCompilation(unsigned int shader, const char *shader_name);
static void checkProgramCompilation(unsigned int program);

// shader methods
static void use(struct Shader *self);
static void delete(Shader *self);
static void setBool(Shader *self,const char* name,bool value);
static void setInt(Shader *self,const char* name,int value);
static void setFloat(Shader *self,const char* name, float value);
static void setMat4(Shader *self, const char* name,mat4 matrix);

Shader shaderConstructor(const char* vertexPath, const char* fragmentPath){
    
    FILE *vertexShaderFile = fopen(vertexPath, "rb");
    FILE *fragmentShaderFile = fopen(fragmentPath, "rb");

    if (!vertexShaderFile) {
        printf("Vertex shader not found: %s\n", vertexPath);
        glfwTerminate();
        exit(1);
    }

    if (!fragmentShaderFile) {
        printf("Fragment shader not found: %s\n", fragmentPath);
        fclose(vertexShaderFile);
        glfwTerminate();
        exit(1);
    }

    fseek(vertexShaderFile, 0, SEEK_END);
    long vertexLength = ftell(vertexShaderFile);
    rewind(vertexShaderFile);

    fseek(fragmentShaderFile, 0, SEEK_END);
    long fragmentLength = ftell(fragmentShaderFile);
    rewind(fragmentShaderFile);

    char *vertexCode = malloc(vertexLength + 1);
    char *fragmentCode = malloc(fragmentLength + 1);

    if (!vertexCode || !fragmentCode) {
        printf("Memory allocation failed\n");
        fclose(vertexShaderFile);
        fclose(fragmentShaderFile);
        glfwTerminate();
        exit(1);
    }

    fread(vertexCode, 1, vertexLength, vertexShaderFile);
    fread(fragmentCode, 1, fragmentLength, fragmentShaderFile);

    vertexCode[vertexLength] = '\0';
    fragmentCode[fragmentLength] = '\0';

    fclose(vertexShaderFile);
    fclose(fragmentShaderFile);

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vCode = vertexCode;
    glShaderSource(vertex, 1, &vCode, NULL);
    glCompileShader(vertex);
    checkShaderCompilation(vertex, "VERTEX");

    const char* fCode = fragmentCode;
    glShaderSource(fragment, 1, &fCode, NULL);
    glCompileShader(fragment);
    checkShaderCompilation(fragment, "FRAGMENT");


    free(vertexCode);
    free(fragmentCode);

    // LINK SHADERS
    // ------------

    Shader s;

    s.ID = glCreateProgram();
 
    glAttachShader(s.ID,vertex);
    glAttachShader(s.ID,fragment);
    glLinkProgram(s.ID);
    checkProgramCompilation(s.ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // BIND FUNCTIONS
    s.use = use;
    s.delete = delete;
    s.setBool = setBool;
    s.setFloat = setFloat;
    s.setInt = setInt;
    s.setMat4 = setMat4;
    return s;
}

static void use(Shader *self){
    glUseProgram(self->ID);
    self->setInt(self, "texture1", 0); // wall
    //self->setInt(self, "texture2", 1); // decal
}

static void setMat4(Shader *self, const char* name,mat4 matrix){
    float *value_ptr = valuePtr(matrix);
    glUniformMatrix4fv(
        glGetUniformLocation(self->ID,name),
        1, GL_FALSE, value_ptr
    );
    free(value_ptr);
}

static void setBool(Shader *self,const char* name, bool value) {
    glUniform1i(glGetUniformLocation(self->ID,name),(int)value);    
}

static void setInt(Shader *self, const char* name, int value) {
    glUniform1i(glGetUniformLocation(self->ID,name),value);    
}

static void setFloat(Shader *self,const char* name, float value){
    glUniform1f(glGetUniformLocation(self->ID,name),value);    
}

static void delete(Shader *self){
    glDeleteProgram(self->ID);
}

static void checkProgramCompilation(unsigned int program){
    int success;
    char infoLog[512];
    glGetProgramiv(program,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(program,512,NULL,infoLog);
        printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n %s \n",infoLog);
        glfwTerminate();
    }
}

static void checkShaderCompilation(unsigned int shader, const char *shader_name){
    int success;
    char infoLog[512];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(shader,512,NULL,infoLog);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n %s \n",shader_name,infoLog);
        glfwTerminate();
    }
}


