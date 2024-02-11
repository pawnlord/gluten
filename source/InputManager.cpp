#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

// #define GLEW_STATIC
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "../include/InputManager.h"
namespace gluten {
InputInfo *activeInfo = nullptr;

void computeMatricesFromInputs(GLFWwindow* window, Camera& cam, InputInfo& inf) {
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - inf.lastTime);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glfwSetCursorPos(window, width / 2, height / 2);
    cam.horizontalAngle += inf.mouseSpeed * (float)(width / 2 - xpos);
    cam.verticalAngle += inf.mouseSpeed * (float)(height / 2 - ypos);
    cam.verticalAngle = glm::clamp(cam.verticalAngle, -3.14f / 2, 3.14f / 2);
    inf.lastTime = currentTime;

    glm::vec3 direction(cos(cam.verticalAngle) * sin(cam.horizontalAngle), sin(cam.verticalAngle), cos(cam.verticalAngle) * cos(cam.horizontalAngle));

    glm::vec3 right = glm::vec3(sin(cam.horizontalAngle - 3.14f / 2.0f), 0, cos(cam.horizontalAngle - 3.14f / 2.0f));

    glm::vec3 up = glm::cross(right, direction);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.position += direction * deltaTime * inf.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.position -= direction * deltaTime * inf.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.position += right * deltaTime * inf.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.position -= right * deltaTime * inf.speed;
    }
    cam.projectionMatrix = glm::perspective(glm::radians(cam.FoV), 4.0f / 3.0f, 0.1f, 100.0f);

    cam.viewMatrix = glm::lookAt(cam.position, cam.position + direction, up);
}

void registerInputInfo(InputInfo& info){
    activeInfo = &info;
    info.active = &activeInfo;
}

void InputInfo::setBrightnessScalar(float* _brightnessScalar) {
    brightnessScalar = _brightnessScalar;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (activeInfo) {
        if (activeInfo->brightnessScalar != nullptr) {
            (*activeInfo->brightnessScalar) += yoffset;
        }
    }
}

}  // namespace gluten
#endif