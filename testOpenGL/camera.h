#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "vector.h"
#include "matrix.h"

using namespace std;

class Camera
{
public:
    enum CameraActionType : int
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    explicit Camera(vec3f pos,
                    vec3f target,
                    vec3f worldUp,
                    float yaw,
                    float pitch)
    {
        _pos = pos;
        _target = target;
        _worldUp = up;
        _yaw = yaw;
        _pitch = pitch;

        // updateCameraVectors();
    }

private:
    void rebuild()
    {
        // rebuild front direction in world space
        vec3f front;
        // refer to pitch_yaw.png

        front.x = cos(rad(_yaw)) * cos(rad(_pitch));
        front.y = sin(rad(_pitch));
        front.z = sin(rad(_yaw)) * cos(rad(_pitch));

        _worldFront = normalize(front);
    }

    // three attributes to create the view transfrom matrix
    // Camera translation updates _pos
    vec3f _pos;
    vec3f _target;
    vec3f _worldUp;

    vec3f _worldFrontDir;

    // mouse movement
    // will change the front world direction.
    // accumulated from the initial state
    float _pitch; // x
    float _yaw;   // y

    // middle scroll, vertical fov
    float _vFov;
};