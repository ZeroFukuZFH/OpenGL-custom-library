#ifndef Textured_H
#define Textured_H

#include <glad/glad.h>
#include <stddef.h>   // for size_t
#include "../includes/shaders.h"
#include "../includes/textures.h"
#include "../../graphics-math/glm.h"
#include "../camera-obj/camera.h"
#include <stdint.h>

typedef struct Textured {

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indices_count;
    unsigned int transform_location;

    Shader shaders;
    Texture textures;

    void (*draw)(struct Textured *self,Camera *camera, vec3 size, vec3 pos, vec2 rotation, vec3 light);
    void (*destroy)(struct Textured *self);      
} Textured;

/**
 * Creates an Textured with its own VAO, VBO, and EBO.
 *
 * @param vertices     Pointer to vertex data
 * @param vertex_size  Size in bytes of vertex data
 * @param indices      Pointer to index data
 * @param index_size   Size in bytes of index data
 */


Textured TexturedConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size,
    Shader shaders,
    Texture Textures
);

#endif // Textured_H
