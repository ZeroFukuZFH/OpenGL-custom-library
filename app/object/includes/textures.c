#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "textures.h"

static void use(Texture *self);

Texture TextureConstructor(const char** image,unsigned int len_of_img_arr){

    Texture t;
    t.use = use;
    t.len_of_texture_array = len_of_img_arr;
    
    if(len_of_img_arr == 0){
        return t;
    }

    int width;
    int height; 
    int nrChannels;
    
    t.texture = (unsigned int *)malloc(sizeof(unsigned int) * len_of_img_arr);
    

    for(int item = 0; item < len_of_img_arr; item++){
        const char *filename = image[item];
    
        if(!image[item] || image[item][0] == '\0'){
            filename = "error.jpeg";        
        }

        unsigned char *data = stbi_load(filename, &width, &height,&nrChannels,0);
        if(data == NULL){
            perror("Failed to load Texture");
            exit(1);
        }

        glGenTextures(1, &t.texture[item]);
        glBindTexture(GL_TEXTURE_2D,t.texture[item]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    return t;
}

static void use(Texture *self){
    if(self->len_of_texture_array == 0) return;
    for(int item = 0; item < self->len_of_texture_array; item++){
        glActiveTexture(GL_TEXTURE0 + item);
        glBindTexture(GL_TEXTURE_2D,self->texture[item]);
    }
}

