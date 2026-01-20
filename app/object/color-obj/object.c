#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "object.h"
#include "../../graphics-math/glm.h"
#include "../includes/shaders.h"
#include <stdint.h>

static void draw(Object *self,Camera *camera, vec3 size, vec3 pos, vec2 rotation, vec3 light, vec3 obj_color);
static void destroy(Object *self);

Object ObjectConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size
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
        6 * sizeof(float),(void*)0
    );

    glEnableVertexAttribArray(0);
        // texCoor
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float),(void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    obj.shaders = shaderConstructor("object/shaders/object.vs", "object/shaders/object.fs");
    obj.transform_location = glGetUniformLocation(obj.shaders.ID, "transform");
    obj.indices_count = index_size / sizeof(unsigned int);
    obj.draw = draw;
    obj.destroy = destroy;

    return obj;
}

static void draw(Object *self, Camera* camera, vec3 size, vec3 pos, vec2 rotation, vec3 light, vec3 obj_color) {
    self->shaders.use(&self->shaders);
    
    glBindVertexArray(self->VAO);
    
    // Get matrices from camera
    mat4 projection = perspective(radians(camera->zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    mat4 view = camera->GetViewMatrix4(camera);
    
    // Create model matrix
    mat4 model = MatrixConstructorDiagonal(1.0f);
    model = translate(model, pos);
    model = scale(model, size);
    model = rotate(model, radians(rotation.x), (vec3){1.0f, 0.0f, 0.0f});
    model = rotate(model, radians(rotation.y), (vec3){0.0f, 1.0f, 0.0f});
    
    // Set matrices
    self->shaders.setMat4(&self->shaders, "projection", projection);
    self->shaders.setMat4(&self->shaders, "view", view);
    self->shaders.setMat4(&self->shaders, "model", model);

    self->shaders.setVec3(&self->shaders, "viewPos", camera->position);
    // Set lighting properties
    vec3 light_pos = {1.2f, 1.0f, 2.0f};
    vec3 diffuse = scaleV3V3(light, (vec3){0.5f, 0.5f, 0.5f});
    vec3 ambient = scaleV3V3(diffuse, (vec3){0.2f, 0.2f, 0.2f});
    
    // Light properties
    self->shaders.setVec3(&self->shaders, "light.position", light_pos);
    self->shaders.setVec3(&self->shaders, "light.ambient", ambient);
    self->shaders.setVec3(&self->shaders, "light.diffuse", diffuse);
    self->shaders.setVec3(&self->shaders, "light.specular", light);
    
    // Material properties
    self->shaders.setVec3(&self->shaders, "material.ambient", obj_color);
    self->shaders.setVec3(&self->shaders, "material.diffuse", obj_color);
    self->shaders.setVec3(&self->shaders, "material.specular", (vec3){0.5f, 0.5f, 0.5f});
    self->shaders.setFloat(&self->shaders, "material.shininess", 32.0f);
    
    glDrawElements(GL_TRIANGLES, self->indices_count, GL_UNSIGNED_INT, 0);
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