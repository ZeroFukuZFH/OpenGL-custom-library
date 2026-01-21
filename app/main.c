#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "graphics-math/glm.h"
#include "object/color-obj/object.h"
#include "object/camera-obj/camera.h"
#include "object/light-obj/light.h"
#include "object/texture-obj/texture.h"

#include "object/includes/shaders.h"
#include "object/includes/textures.h"

#include "3D-obj-data/cube.h"

GLFWwindow *initEngine(unsigned int width, unsigned int height, const char *title);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// MACROS
// ------

// camera
Camera camera;
float last_x = 800 / 2.0f;
float last_y = 600 / 2.0f;
bool first_mouse = true;

// timing
float delta_time = 0.0f;
float last_frame = 0.0f;

// light
vec3 light_color = {1.0f, 1.0f,1.0f};

int main(void) {

    GLFWwindow *window = initEngine(800,600,"hello world");

    glEnable(GL_DEPTH_TEST);

    camera = CameraConstructor(
        (vec3){0.0f, 0.0f, 3.0f},
        (vec3){0.0f, 1.0f, 0.0f},
        YAW, PITCH, SPEED, SENSITIVITY, ZOOM
    );

    const char *textures[] = { "object/texture/container.jpg", "object/texture/Awesome_Face.png" };

    Object obj = ObjectConstructor(CUBE, sizeof(CUBE_WITH_TEXTURE),CUBE_DATA, sizeof(CUBE_DATA));
    
    Light light = LightConstructor(CUBE, sizeof(CUBE),CUBE_DATA, sizeof(CUBE_DATA));
    
    Textured texturedObj = TexturedConstructor(
        CUBE_WITH_TEXTURE, sizeof(CUBE_WITH_TEXTURE),CUBE_DATA, sizeof(CUBE_DATA),
        shaderConstructor("object/shaders/texture.vs","object/shaders/texture.fs"),
        TextureConstructor(textures,sizeof(textures)/sizeof(textures[0]))
    );

    // DISABLE ANYTIME -- WIREFRAME MODE
    // ---------------------------------
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw lit object
        obj.draw(&obj, &camera,
                (vec3){2.0f, 2.0f, 2.0f}, // size
                (vec3){0 + cos(current_frame) * 8, 0 + sin(current_frame) * 8, 0.0f},  // Position
                (vec2){current_frame * 10, current_frame * 10}, // Rotation
                light_color,                // Light color (white)
                (vec3){1.0f, 0.5f, 0.31f}); // Object color (orange)
        
        // Draw light cube (smaller, at light position)
        light.draw(&light, &camera,
                (vec3){1.0f, 1.0f, 1.0f}, // size
                (vec3){3.0f, 0.0f, 0.0f},// Position at light source
                (vec2){0.0f, 0.0f},   // No rotation
                light_color,          // Emissive color
                light_color);   

        texturedObj.draw(&texturedObj,&camera,
                (vec3){2.0f, 2.0f, 2.0f}, // size
                (vec3){6.0f, 0.0f, 0.0f},  // Position
                (vec2){current_frame * 10, current_frame * 10}, // Rotation
                light_color);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    light.destroy(&light);
    texturedObj.destroy(&texturedObj);
    obj.destroy(&obj);

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
    
    float x_pos = (float)xpos;
    float y_pos = (float)ypos;

    if(first_mouse){
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float xoffset = x_pos - last_x;
    float yoffset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    camera.ProcessMouseMovement(&camera, xoffset, yoffset,true);

}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(&camera, (float)yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(&camera, FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(&camera, BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(&camera, LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(&camera, RIGHT, delta_time);
}

GLFWwindow *initEngine(unsigned int width, unsigned int height, const char *title){
    
    if (!glfwInit()) {
        perror("Failed to initialize GLFW");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        perror("Failed to create GLFW window");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // DISABLE ANYTIME - HIDES CURSOR
    // ------------------------------
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        perror("Failed to initialize GLAD");
        exit(1);
    }

    return window;
}
