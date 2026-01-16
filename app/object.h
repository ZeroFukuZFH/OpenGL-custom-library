#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <stddef.h>   // for size_t
#include "shaders.h"
#include "textures.h"
#include "glm.h"
#include <stdint.h>

typedef struct Color {
    u_int8_t R;
    u_int8_t G;
    u_int8_t B;
    u_int8_t A;
} Color;

/**
 * Color struct with RGBA values.
 *
 * @param R - Red 0 to 255
 * @param G - Green 0 to 255
 * @param B - Blue 0 to 255
 * @param A - Alpha 0 to 255
*/

Color ColorConstructor(
    u_int8_t R,
    u_int8_t G,
    u_int8_t B,
    u_int8_t A
);
typedef struct Object {

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indices_count;
    unsigned int transform_location;

    Shader shaders;
    Texture textures;

    void (*draw)(struct Object *self,float x, float y, float rotation);
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
