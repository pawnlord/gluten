// #define GLEW_STATIC
#include <stdio.h>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

namespace im {
    void computeMatricesFromInputs(GLFWwindow* window);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void setBrightnessScalar(float *_brightnessScalar);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}