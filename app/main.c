#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "shaders.h"
#include "textures.h"
#include "glm.h"
#include "object.h"
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
    
    float verticesA[] = { 
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indicesA[] = {
        0, 1, 3,
        1, 2, 3
    };

    float verticesB[] = { 
        // positions          // colors           // texture coords
        1.0f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
        1.0f, -0.5f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
        0.0f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,   0.5f, 0.5f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indicesB[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    Object obj1 = ObjectConstructor(
        verticesA,sizeof(verticesA),
        indicesA, sizeof(indicesA),
        shaderConstructor("shaders/shader.vs", "shaders/shader.fs"),
        textureConstructor("wall.jpg")
    );

    Object obj2 = ObjectConstructor(
        verticesB,sizeof(verticesB),
        indicesB, sizeof(indicesB),
        shaderConstructor("shaders/shader.vs", "shaders/shader.fs"),
        textureConstructor("")
    );
    
    // UNCOMMENT : if need to convert to wireframe
    // -------------------------------------------
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    while (!glfwWindowShouldClose(main_window)){

        // inputs
        processInput(main_window);

        // bg-color
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render triangle / object
        obj2.draw(&obj2);
        obj1.draw(&obj1);

        // GPU stuff
        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }

    // OPTIONAL : DE-ALLOC AFTER USE 
    // -----------------------------
    obj2.destroy(&obj2);
    obj1.destroy(&obj1);

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

