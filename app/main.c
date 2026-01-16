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
void processInput(GLFWwindow *window, float *control_x , float *control_y );

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

    glEnable(GL_DEPTH_TEST);

        float vertices[] = {
        // positions        // texcoords
        // front
        -0.5f,-0.5f, 0.5f, 0.0f,0.0f,
        0.5f,-0.5f, 0.5f, 1.0f,0.0f,
        0.5f, 0.5f, 0.5f, 1.0f,1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,1.0f,

        // back
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f,
        0.5f,-0.5f,-0.5f, 1.0f,0.0f,
        0.5f, 0.5f,-0.5f, 1.0f,1.0f,
        -0.5f, 0.5f,-0.5f, 0.0f,1.0f,

        // left
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f,
        -0.5f, 0.5f,-0.5f, 1.0f,0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f,1.0f,
        -0.5f,-0.5f, 0.5f, 0.0f,1.0f,

        // right
        0.5f,-0.5f,-0.5f, 0.0f,0.0f,
        0.5f, 0.5f,-0.5f, 1.0f,0.0f,
        0.5f, 0.5f, 0.5f, 1.0f,1.0f,
        0.5f,-0.5f, 0.5f, 0.0f,1.0f,

        // bottom
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f,
        0.5f,-0.5f,-0.5f, 1.0f,0.0f,
        0.5f,-0.5f, 0.5f, 1.0f,1.0f,
        -0.5f,-0.5f, 0.5f, 0.0f,1.0f,

        // top
        -0.5f, 0.5f,-0.5f, 0.0f,0.0f,
        0.5f, 0.5f,-0.5f, 1.0f,0.0f,
        0.5f, 0.5f, 0.5f, 1.0f,1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,1.0f
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0,       // front
        4,5,6, 6,7,4,       // back
        8,9,10,10,11,8,     // left
        12,13,14,14,15,12, // right
        16,17,18,18,19,16, // bottom
        20,21,22,22,23,20  // top
    };

    Object obj1 = ObjectConstructor(
        vertices,sizeof(vertices),
        indices, sizeof(indices),
        shaderConstructor("shaders/shader.vs", "shaders/shader.fs"),
        textureConstructor("container.jpg")
    );

    // UNCOMMENT : if need to convert to wireframe
    // -------------------------------------------
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    float control_x = -1.0f;
    float control_y = -1.0f;
    float rotation = -1.0f;

    /* REGISTER CALLBACKS ONCE */
    glfwSetWindowUserPointer(main_window, &rotation);
    glfwSetScrollCallback(main_window, scroll_callback);
    glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(main_window)){
        // inputs
        processInput(main_window, &control_x, &control_y);
        
        // bg-color/
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render triangle / object

        obj1.draw(&obj1, control_x, control_y, rotation);

        // GPU stuff
        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }

    // OPTIONAL : DE-ALLOC AFTER USE 
    // -----------------------------
    obj1.destroy(&obj1);

    // close window
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0,0,width,height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    float* control_y = (float*)glfwGetWindowUserPointer(window);

    if (control_y)
    {
        *control_y += (float)yoffset * 0.05f;
    }
}

void processInput(GLFWwindow *window, float *control_x, float *control_y){

    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        *control_y += 0.01f;
        //printf("y : %f\n", *control_y);
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        *control_x -= 0.01f;
        //printf("x : %f\n", *control_x);
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        *control_y -= 0.01f;
        //printf("y : %f\n", *control_y);
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        *control_x += 0.01f;
        //printf("x : %f\n", *control_x);
    }
    
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        printf("Escape key has been pressed!\n");
        glfwSetWindowShouldClose(window,true);
    }
    
}   

