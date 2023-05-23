#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H
#include <functional>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

namespace gm {

    class GltnGraphicsContext {
        public: 
            GltnGraphicsContext(int width, int height, std::string name, float projectionAngle);
            GLFWwindow* window; 
            GLuint VertexArrayID;
 	        glm::mat4 projection;
 	        glm::mat4 view;
            int width, height;
            float brightnessScalar = 1.0;
    };

}
#endif