#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "textures.h"

static void use(Texture *self);

Texture textureConstructor(const char* image){

    const char *filename = image;
    
    if(!image || image[0] == '\0'){
        filename = "error.jpeg";        
    }

    int width;
    int height; 
    int nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height,&nrChannels,0);
    
    if(data == NULL){
        perror("Failed to load texture");
        exit(1);
    }

    Texture t;
    t.use = use;
    
    

    glGenTextures(1, &t.texture1);
    glBindTexture(GL_TEXTURE_2D,t.texture1);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return t;
}

static void use(Texture *self){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,self->texture1);
}

