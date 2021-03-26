#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define GLEW_STATIC
#include <stdio.h>
#include <iostream>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

#include "../include/InputManager.h"

namespace im {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    double lastTime = 0;
    // position
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    // horizontal angle : toward -Z
    float horizontalAngle = (2*3.14f)/2+(3.14f/3);
    // vertical angle : 0, look at the horizon
    float verticalAngle = 0.0f;
    // Initial Field of View
    float FoV = 45.0f;
    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 1.0f;
    void computeMatricesFromInputs(GLFWwindow* window){
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Get the window dimensions
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, width/2, height/2);
        // Compute new orientation
        horizontalAngle += mouseSpeed * deltaTime * float( width/2 - xpos );
        verticalAngle   += mouseSpeed * deltaTime * float( height/2 - ypos );
        verticalAngle = glm::clamp(verticalAngle, -3.14f/2, 3.14f/2);
        lastTime = currentTime;

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        // Right vector
        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );

        // Up vector : perpendicular to both direction and right
        glm::vec3 up = glm::cross( right, direction );
        // Move forward
        if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }
        // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        viewMatrix       = glm::lookAt(
            position,           // Camera is here
            position+direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
        );
    }
    glm::mat4 getViewMatrix(){
        return viewMatrix;
    }
    glm::mat4 getProjectionMatrix(){
        return projectionMatrix;
    }
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
        FoV += yoffset;
        printf("callback called, %f %f %f\n", FoV, xoffset, yoffset);
    }
}
#endif