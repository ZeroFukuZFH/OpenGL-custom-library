#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <stddef.h>   // for size_t
#include "shaders.h"
#include "textures.h"

typedef struct Object {

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indices_count;
    unsigned int transform_location;

    Shader shaders;
    Texture textures;

    void (*draw)(struct Object *self);
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
    unsigned int *indices, size_t index_size,
    Shader shaders,
    Texture textures
);

#endif // OBJECT_H
