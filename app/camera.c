#include <glad/glad.h>
#include <math.h>
#include "camera.h"


static void updateCameraVectors(Camera *self);
static void ProcessKeyboard(Camera *self, Camera_Movement direction, float delta_time);
static void ProcessMouseMovement(Camera *self, float x_offset, float y_offset, GLboolean constrain_pitch);
static void ProcessMouseScroll(Camera *self, float y_offset);
static mat4 GetViewMatrix4(Camera *self);
static void updateCameraVectors(Camera *self);

Camera CameraConstructor(
    vec3 position,
    vec3 up,
    float yaw,
    float pitch,
    float movement_speed,
    float mouse_sensitivity,
    float zoom
){
    Camera camera;
    camera.position = position;
    camera.world_up = up;
    camera.yaw = yaw;
    camera.pitch = pitch;
    camera.movement_speed = movement_speed;
    camera.mouse_sensitivity = mouse_sensitivity;
    camera.zoom = zoom;

    // method binding
    camera.GetViewMatrix4 = GetViewMatrix4;
    camera.ProcessKeyboard = ProcessKeyboard;
    camera.ProcessMouseMovement = ProcessMouseMovement;
    camera.ProcessMouseScroll = ProcessMouseScroll;

    updateCameraVectors(&camera);
    return camera;
}

static void ProcessKeyboard(Camera *self, Camera_Movement direction, float delta_time){
    float velocity = self->movement_speed * delta_time;

    if (direction == FORWARD) {
        self->position = add(
            self->position,
            scaleV(self->front, velocity)
        );
    }

    if (direction == BACKWARD) {
        self->position = minus(
            self->position,
            scaleV(self->front, velocity)
        );
    }

    if (direction == LEFT) {
        self->position = minus(
            self->position,
            scaleV(self->right, velocity)
        );
    }

    if (direction == RIGHT) {
        self->position = add(
            self->position,
            scaleV(self->right, velocity)
        );
    }
}


static void ProcessMouseMovement(Camera *self, float x_offset, float y_offset, GLboolean constrain_pitch){
    x_offset *= self->mouse_sensitivity;
    y_offset *= self->mouse_sensitivity;
    
    self->yaw += x_offset;
    self->pitch += y_offset;

    if(constrain_pitch){
        if (self->pitch > 89.0f)
            self->pitch = 89.0f;
        if (self->pitch < -89.0f)
            self->pitch = -89.0f;
    }

    updateCameraVectors(self);
}

static void ProcessMouseScroll(Camera *self, float y_offset){
    self->zoom -= (float)y_offset;
    if(self->zoom < 1.0f){
        self->zoom = 1.0f;
    }
    if(self->zoom > 45.0f){
        self->zoom = 45.0f;
    }
}

static mat4 GetViewMatrix4(Camera *self){
    return lookAt(self->position, add(self->position ,self->front), self->up);
}

static void updateCameraVectors(Camera *self){
    vec3 front;
    front.x = cos(radians(self->yaw)) * cos(radians(self->pitch));
    front.y = sin(radians(self->pitch));
    front.z = sin(radians(self->yaw)) * cos(radians(self->pitch));
    self->front = normalize(front);
    self->right = normalize(cross(self->front,self->world_up));
    self->up = normalize(cross(self->right,self->front));
}