#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "object.h"
#include "glm.h"

static void draw(Object *self);
static void destroy(Object *self);

Object ObjectConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size,
    Shader shaders,
    Texture textures
){
    Object obj;

    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glGenBuffers(1, &obj.EBO);
    glBindVertexArray(obj.VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_DYNAMIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_DYNAMIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    obj.transform_location = glGetUniformLocation(shaders.ID, "transform");
    obj.indices_count = index_size / sizeof(unsigned int);
    obj.shaders = shaders;
    obj.textures = textures;
    obj.draw = draw;
    obj.destroy = destroy;

    return obj;
}


static void draw(Object *self){
    
    self->shaders.use(&self->shaders);
    self->textures.use(&self->textures);

    glBindVertexArray(self->VAO);
    self->shaders.setFloat(&self->shaders,"someUniform",1.0f);

    // object transform
    mat4 trans = MatrixConstructorDiagonal(1.0f);
    trans = rotate(trans, radians((float)glfwGetTime()),(vec3){0.0f,0.0f,1.0});
    float *value_ptr = valuePtr(trans); // needs to de-alloc because of dynamic implementation
    glUniformMatrix4fv(self->transform_location, 1, GL_FALSE, value_ptr);
    free(value_ptr);

    glDrawElements(GL_TRIANGLES,self->indices_count,GL_UNSIGNED_INT, 0);
}

static void destroy(Object *self){
    glDeleteVertexArrays(1,&self->VAO);
    glDeleteBuffers(1,&self->VBO);
    glDeleteBuffers(1,&self->EBO);
    self->shaders.delete(&self->shaders);
    self->VAO = 0;
    self->VBO = 0;
    self->EBO = 0;
    self->indices_count = 0;
}