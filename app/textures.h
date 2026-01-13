#ifndef TEXTURES_H
#define TEXTURES_H

typedef struct Textures {
    unsigned int texture;
    void(*use)(struct Textures *self);
}Texture;

Texture textureConstructor(const char* image);

#endif

