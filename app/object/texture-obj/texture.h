#ifndef Textured_H
#define Textured_H

#include <glad/glad.h>
#include <stddef.h>   // for size_t
#include "../includes/shaders.h"
#include "../includes/textures.h"
#include "../../graphics-math/glm.h"
#include "../camera-obj/camera.h"
#include <stdint.h>
struct DirectionalLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    struct DirectionalLight light;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    struct PointLight light;
    float cutoff;
    float outerCutoff;
};
typedef struct Textured {

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indices_count;
    unsigned int transform_location;

    float x,y,z;
    float rx, ry;
    float sx, sy, sz;

    float R,G,B,A;

    Shader shaders;
    Texture textures;

    void (*setDirectionalLighting)(struct DirectionalLight *dirLight);
    void (*setPointLighting)(struct PointLight *pointLight);
    void (*setSpotLight)(struct SpotLight *spotLight);
    
    void (*setScale)(struct Textured *self, vec3 size);
    void (*setPosition)(struct Textured *self, vec3 position);
    void (*setRotation)(struct Textured *self, vec2 rotation);

    void (*setPosX)(struct Textured *self, float x);
    void (*setPosY)(struct Textured *self, float y);
    void (*setPosZ)(struct Textured *self, float z);

    void (*setScaleX)(struct Textured *self, float x);
    void (*setScaleY)(struct Textured *self, float y);
    void (*setScaleZ)(struct Textured *self, float z);

    void (*setRotationX)(struct Textured *self, float x);
    void (*setRotationY)(struct Textured *self, float y);

    void (*setColor)(struct Textured *self, vec4 color);

    void (*draw)(struct Textured *self,Camera *camera, vec3 light, vec3 light_pos);
    void (*destroy)(struct Textured *self);      
} Textured;

Textured TexturedConstructor(
    float *vertices, size_t vertex_size,
    unsigned int *indices, size_t index_size,
    Shader shaders,
    Texture Textures
);

#endif // Textured_H
