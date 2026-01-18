#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "object.h"
#include "glm.h"
#include "shaders.h"
#include <stdint.h>

Color ColorConstructor(
    u_int8_t R,
    u_int8_t G,
    u_int8_t B,
    u_int8_t A
){
    // ---------------------------------------
    // this may seem redundant, but it is just 
    // to normalize how we construct objects.
    // ---------------------------------------

    return (Color){R,G,B,A};
}

static void use(Object *self);
static void draw(Object *self, float x, float y, float z, float rotation_x, float rotation_y);
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
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_STATIC_DRAW);

    // Vertex attributes
        // position
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);
        // texCoor
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    obj.transform_location = glGetUniformLocation(shaders.ID, "transform");
    obj.indices_count = index_size / sizeof(unsigned int);
    obj.shaders = shaders;
    obj.textures = textures;
    obj.use = use;
    obj.draw = draw;
    obj.destroy = destroy;

    return obj;
}

static void use(Object *self){
    self->shaders.use(&self->shaders);
    self->textures.use(&self->textures);
}

static void draw(Object *self, float x, float y, float z, float rotation_x, float rotation_y){

    glBindVertexArray(self->VAO);
    self->shaders.setFloat(&self->shaders,"someUniform",1.0f);

    mat4 view = MatrixConstructorDiagonal(1.0f);
    mat4 projection = MatrixConstructorDiagonal(1.0f);
    
    projection = perspective(radians(45.0f),800.0f / 600.0f, 0.1f, 100.0f);
    view = translate(view, (vec3){0.0f,0.0f,-3.0f});

    self->shaders.setMat4(&self->shaders,"projection", projection);
    self->shaders.setMat4(&self->shaders,"view", view);

    unsigned int model_location = glGetUniformLocation(self->shaders.ID, "model");
    unsigned int view_location = glGetUniformLocation(self->shaders.ID, "view");
    
    mat4 model = MatrixConstructorDiagonal(1.0f);
    model = translate(model,(vec3){x,y,z});
    model = rotate(model,radians(rotation_x),(vec3){1.0f, 0.0f, 0.0f});
    model = rotate(model,radians(rotation_y),(vec3){0.0f, 1.0f, 0.0f});
    self->shaders.setMat4(&self->shaders,"model", model);

    glUniformMatrix4fv(model_location, 1, GL_FALSE, &model.s1.x);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &view.s1.x);

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