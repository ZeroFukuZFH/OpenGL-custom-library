#ifndef TEXTURES_H
#define TEXTURES_H

typedef struct Textures {
    unsigned int len_of_texture_array;
    unsigned int *texture;
    void(*use)(struct Textures *self);
}Texture;

Texture TextureConstructor(const char** image, unsigned int len_of_img_arr);

#endif

