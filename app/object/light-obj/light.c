#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "light.h"
#include "../../graphics-math/glm.h"
#include "../../properties.h"
#include "../includes/shaders.h"
#include <stdint.h>

static void setScale(struct Light *self, vec3 size);
static void setPosition(struct Light *self, vec3 position);
static void setRotation(struct Light *self, vec2 rotation);

static void setPosX(struct Light *self, float x);
static void setPosY(struct Light *self, float y);
static void setPosZ(struct Light *self, float z);

static void setScaleX(struct Light *self, float x);
static void setScaleY(struct Light *self, float y);
static void setScaleZ(struct Light *self, float z);

static void setRotationX(struct Light *self, float x);
static void setRotationY(struct Light *self, float y);

static void draw(Light *self,Camera *camera, vec3 light, vec3 light_color);
static void destroy(Light *self);

Light LightConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size
){
    
    Light light;

    glGenVertexArrays(1, &light.VAO);
    glGenBuffers(1, &light.VBO);
    glGenBuffers(1, &light.EBO);
    glBindVertexArray(light.VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, light.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_STATIC_DRAW);

    // Vertex attributes
        // position
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);
        // texCoor
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    light.shaders = shaderConstructor("object/shaders/light.vs", "object/shaders/light.fs");
    light.transform_location = glGetUniformLocation(light.shaders.ID, "transform");
    light.indices_count = index_size / sizeof(unsigned int);
    
    light.setScale = setScale;
    light.setPosition = setPosition;
    light.setRotation = setRotation;
    light.setPosX = setPosX;
    light.setPosY = setPosY;
    light.setPosZ = setPosZ;
    light.setScaleX = setScaleX;
    light.setScaleY = setScaleY;
    light.setScaleZ = setScaleZ;
    light.setRotationX = setRotationX;
    light.setRotationY = setRotationY;

    light.draw = draw;
    light.destroy = destroy;

    return light;
}

static void draw(Light *self, Camera* camera, vec3 light, vec3 light_color) {
    self->shaders.use(&self->shaders);
    
    glBindVertexArray(self->VAO);
    
    // Get matrices from camera
    mat4 projection = perspective(radians(camera->zoom), (float)PROJECT_WINDOW_WIDTH/(float)PROJECT_WINDOW_HEIGHT, 0.1f, 100.0f);
    mat4 view = camera->GetViewMatrix4(camera);
    
    // Create model matrix
    mat4 model = MatrixConstructorDiagonal(1.0f);
    model = translate(model, (vec3){self->x,self->y,self->z});
    model = scale(model, (vec3){self->sx, self->sy, self->sz});
    model = rotate(model, radians(self->rx), (vec3){1.0f, 0.0f, 0.0f});
    model = rotate(model, radians(self->ry), (vec3){0.0f, 1.0f, 0.0f});
    
    // Set matrices
    self->shaders.setMat4(&self->shaders, "projection", projection);
    self->shaders.setMat4(&self->shaders, "view", view);
    self->shaders.setMat4(&self->shaders, "model", model);
    
    self->shaders.setVec3(&self->shaders, "lightColor", light);
    // Set lighting properties
    vec3 light_pos = {1.2f, 1.0f, 2.0f};
    vec3 diffuse = scaleV3V3(light, (vec3){0.5f, 0.5f, 0.5f});
    vec3 ambient = scaleV3V3(diffuse, (vec3){0.2f, 0.2f, 0.2f});
    
    // Light properties
    self->shaders.setVec3(&self->shaders, "light.position", light_pos);
    self->shaders.setVec3(&self->shaders, "light.ambient", ambient);
    self->shaders.setVec3(&self->shaders, "light.diffuse", diffuse);
    self->shaders.setVec3(&self->shaders, "light.specular", light);
    
    
    glDrawElements(GL_TRIANGLES, self->indices_count, GL_UNSIGNED_INT, 0);
}

static void setScale(struct Light *self, vec3 size) {
    self->sx = size.x;
    self->sy = size.y;
    self->sz = size.z;
}

static void setPosition(struct Light *self, vec3 position) {
    self->x = position.x;
    self->y = position.y;
    self->z = position.z;
}

static void setRotation(struct Light *self, vec2 rotation) {
    self->rx = rotation.x;
    self->ry = rotation.y;
}

static void setPosX(struct Light *self, float x) { self->x = x; }
static void setPosY(struct Light *self, float y) { self->y = y; }
static void setPosZ(struct Light *self, float z) { self->z = z; }

static void setScaleX(struct Light *self, float x) { self->sx = x; }
static void setScaleY(struct Light *self, float y) { self->sy = y; }
static void setScaleZ(struct Light *self, float z) { self->sz = z; }

static void setRotationX(struct Light *self, float x) { self->rx = x; }
static void setRotationY(struct Light *self, float y) { self->ry = y; }

static void destroy(Light *self){
    glDeleteVertexArrays(1,&self->VAO);
    glDeleteBuffers(1,&self->VBO);
    glDeleteBuffers(1,&self->EBO);
    self->shaders.destroy(&self->shaders);
    self->VAO = 0;
    self->VBO = 0;
    self->EBO = 0;
    self->indices_count = 0;
}