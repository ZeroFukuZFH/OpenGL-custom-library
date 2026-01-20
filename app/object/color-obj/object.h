#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <stddef.h>   // for size_t
#include "../includes/shaders.h"
#include "../includes/textures.h"
#include "../../graphics-math/glm.h"
#include "../camera-obj/camera.h"
#include <stdint.h>
typedef struct Object {

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indices_count;
    unsigned int transform_location;

    Shader shaders;

    void (*draw)(struct Object *self,Camera *camera, vec3 size, vec3 pos, vec2 rotation, vec3 light, vec3 obj_color);
    void (*destroy)(struct Object *self);      
} Object;

/**
 * Creates an Object with its own VAO, VBO, and EBO.
 *
 * @param vertices     Pointer to vertex data
 * @param vertex_size  Size in bytes of vertex data
 * @param indices      Pointer to index data
 * @param index_size   Size in bytes of index data
 */


Object ObjectConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size
);

#endif // OBJECT_H
