#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


namespace gluten {
    class Camera {
        public:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::vec3 position = glm::vec3(0, 0, 0);
        float horizontalAngle = (2 * 3.14f) / 2 + (3.14f / 3);
        float verticalAngle = 0;
        float FoV = 60.0f;


        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
    };
}

#endif // CAMERA_H