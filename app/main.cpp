#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "properties.h"

#include "graphics-math/glm.h"
#include "object/camera-obj/camera.h"
#include "object/light-obj/light.h"
#include "object/texture-obj/texture.h"

#include "object/includes/shaders.h"
#include "object/includes/textures.h"

#include "3D-obj-data/cube.h"
#include "3D-obj-data/plane.h"

#include "GUI/gui.hpp"

GLFWwindow *initEngine(unsigned int width, unsigned int height, const char *title);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
Camera camera;
float last_x = PROJECT_WINDOW_WIDTH / 2.0f;
float last_y = PROJECT_WINDOW_HEIGHT / 2.0f;
bool first_mouse = true;

// timing
float delta_time = 0.0f;
float last_frame = 0.0f;

// light
vec3 light_color = {1.0f, 1.0f,1.0f};
vec3 light_pos = {1.0f,1.0f,1.0f};

int main(int argc,char **argv) {

    GUI gui;

    GLFWwindow *window = initEngine(PROJECT_WINDOW_WIDTH,PROJECT_WINDOW_HEIGHT,"hello world");

    glEnable(GL_DEPTH_TEST);

    camera = CameraConstructor(
        (vec3){0.0f, 0.0f, 3.0f},
        (vec3){0.0f, 1.0f, 0.0f},
        YAW, PITCH, SPEED, SENSITIVITY, ZOOM
    );

    const char *textures[] = { 
        "object/texture/error.jpeg"
    };

    Light light = LightConstructor(CUBE, sizeof(CUBE),CUBE_DATA, sizeof(CUBE_DATA));
    
    Textured texturedObj = TexturedConstructor(
        CUBE_WITH_TEXTURE, sizeof(CUBE_WITH_TEXTURE),CUBE_DATA, sizeof(CUBE_DATA),
        shaderConstructor("object/shaders/texture.vs","object/shaders/texture.fs"),
        TextureConstructor(textures,sizeof(textures)/sizeof(textures[0]))
    );

    Textured plane = TexturedConstructor(
        PLANE, sizeof(PLANE),PLANE_DATA, sizeof(PLANE_DATA),
        shaderConstructor("object/shaders/texture.vs","object/shaders/texture.fs"),
        TextureConstructor(textures,sizeof(textures)/sizeof(textures[0]))
    );

    // DISABLE ANYTIME -- WIREFRAME MODE
    // ---------------------------------
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        
        gui.begin();

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        light.setPosition(&light, light_pos);
        light.setRotation(&light, (vec2){0.0f,0.0f});
        light.setScale(&light,(vec3){0.25f,0.25f,0.25f});
        light.draw(&light, &camera,light_color,light_color);   

        texturedObj.setPosition(&texturedObj, (vec3){2.0f,1.0f,1.0f});
        texturedObj.setRotation(&texturedObj, (vec2){0.0f,0.0f});
        texturedObj.setScale(&texturedObj,(vec3){0.25f,0.25f,0.25f});
        texturedObj.setColor(&texturedObj,(vec4){1.0f, 0.5f, 0.31f,0.5f});
        texturedObj.draw(&texturedObj,&camera,light_color, light_pos);

        plane.setPosition(&plane, (vec3){2.0f,0.0f,1.0f});
        plane.setRotation(&plane, (vec2){0.0f,0.0f});
        plane.setScale(&plane,(vec3){5.0f,0.0f,5.0f});
        plane.setColor(&plane,(vec4){1.0f, 1.0f, 1.0f,1.0f});
        plane.draw(&plane,&camera,light_color, light_pos);

        light_pos.y = 0 + cos((float)glfwGetTime()) * 5;

        gui.someWindow("scene",0,0,(float)PROJECT_WINDOW_WIDTH * 0.25f, (float)PROJECT_WINDOW_HEIGHT);

        glfwSwapBuffers(window);
        glfwPollEvents();        
    }

    gui.destroy();

    light.destroy(&light);
    texturedObj.destroy(&texturedObj);
    plane.destroy(&plane);

    
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
    ImGuiIO& io = ImGui::GetIO();
    
    if (io.WantCaptureMouse) {
        return;  
    }

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

     ImGuiIO& io = ImGui::GetIO();
    
    if (io.WantCaptureMouse && io.WantCaptureKeyboard) return;

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls 
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    return window;
}
