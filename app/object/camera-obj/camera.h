#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include "../../graphics-math/glm.h"

typedef enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
} Camera_Movement;

// DEFAULT CAMERA VALUES
static const float YAW         = -90.0f;
static const float PITCH       =  0.0f;
static const float SPEED       =  2.5f;
static const float SENSITIVITY =  0.1f;
static const float ZOOM        =  45.0f;

typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    mat4(* GetViewMatrix4)(struct Camera *self);
    void(* ProcessKeyboard)(struct Camera *self, Camera_Movement direction, float delta_time);
    void(* ProcessMouseMovement)(struct Camera *self,float x_offset, float y_offset, GLboolean constrain_pitch);
    void(* ProcessMouseScroll)(struct Camera *self,float y_offset);

} Camera;

Camera CameraConstructor(
    vec3 position,
    vec3 up,
    float yaw,
    float pitch,
    float movement_speed,
    float mouse_sensitivity,
    float zoom
);

#endif