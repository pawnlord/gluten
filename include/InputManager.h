// #define GLEW_STATIC
#include <stdio.h>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

#include "Camera.h"

namespace gluten {
    struct InputInfo {
        double lastTime = 0;
        float speed = 3.0f;  // 3 units / second
        float mouseSpeed = 0.007f;
        float* brightnessScalar = nullptr;
        InputInfo **active = nullptr;
        void setBrightnessScalar(float *_brightnessScalar);
        ~InputInfo() {
            if (active) {
                *active = nullptr; 
            }
        }
    };

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void computeMatricesFromInputs(GLFWwindow* window, Camera& cam, InputInfo& inf);
    void registerInputInfo(InputInfo& info);
}