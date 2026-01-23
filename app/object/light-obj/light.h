#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <stddef.h>   // for size_t
#include "../includes/shaders.h"
#include "../includes/textures.h"
#include "../../graphics-math/glm.h"
#include "../camera-obj/camera.h"
#include <stdint.h>
typedef struct Light {

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indices_count;
    unsigned int transform_location;

    float x,y,z;
    float rx, ry;
    float sx, sy, sz;
    
    Shader shaders;

    void (*setScale)(struct Light *self, vec3 size);
    void (*setPosition)(struct Light *self, vec3 position);
    void (*setRotation)(struct Light *self, vec2 rotation);

    void (*setPosX)(struct Light *self, float x);
    void (*setPosY)(struct Light *self, float y);
    void (*setPosZ)(struct Light *self, float z);

    void (*setScaleX)(struct Light *self, float x);
    void (*setScaleY)(struct Light *self, float y);
    void (*setScaleZ)(struct Light *self, float z);

    void (*setRotationX)(struct Light *self, float x);
    void (*setRotationY)(struct Light *self, float y);
    
    void (*draw)(struct Light *self,Camera *camera, vec3 light, vec3 light_color);
    void (*destroy)(struct Light *self);      
} Light;

/**
 * Creates an Light with its own VAO, VBO, and EBO.
 *
 * @param vertices     Pointer to vertex data
 * @param vertex_size  Size in bytes of vertex data
 * @param indices      Pointer to index data
 * @param index_size   Size in bytes of index data
 */


Light LightConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size
);

#endif // Light_H
