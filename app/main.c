#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "shaders.h"
#include "textures.h"
#include "glm.h"
// to resize viewport during resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// escape key close
void processInput(GLFWwindow *window);

int main(void){

    if(!glfwInit()){
        perror("Failed to initialize GLFW library");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
    #endif

    GLFWwindow *main_window = glfwCreateWindow(600,400,"hello world",NULL,NULL);

    if(main_window == NULL){
        glfwTerminate();
        perror("Failed to create GLFW window!");
        return -1;
    }

    glfwMakeContextCurrent(main_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        perror("Failed to initialize GLAD\n");
        return -1;
    }

    // SHADERS

    Shader myShader = shaderConstructor("shaders/shader.vs", "shaders/shader.fs");
    Texture myTexture = textureConstructor("wall.jpg");
    
    float vertices[] = { 
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // dynamic draw if triangle position changes alot
    // ----------------------------------------------
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    // dynamic draw if triangle position changes alot
    // ----------------------------------------------
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    // position
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load textures
   

    // UNCOMMENT : if need to convert to wireframe
    // -------------------------------------------
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    while (!glfwWindowShouldClose(main_window)){

        // inputs
        processInput(main_window);

        // bg-color
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        
        myTexture.use(&myTexture);
        myShader.use(&myShader);
        myShader.setFloat(&myShader,"someUniform",1.0f);

        // render triangle / object
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

        // GPU stuff
        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }

    // OPTIONAL : DE-ALLOC AFTER USE 
    // -----------------------------
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    myShader.delete(&myShader);

    // close window
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        printf("Escape key has been pressed!\n");
        glfwSetWindowShouldClose(window,true);
    }
}

