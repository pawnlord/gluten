#define GLEW_STATIC
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

namespace gm {
    GLuint setupGraphicsManager(GLFWwindow* window, glm::mat4& Projection, float projectionAngle);
    GLuint drawArray(int attribLocation, int attribSize, 
                        glm::mat4 Projection,  glm::mat4 View, glm::mat4 Model,
                        GLfloat g_vertex_buffer_data[], GLfloat g_color_buffer_data[],
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size
                    );
}