
#define GLEW_STATIC
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
#include <vector>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

namespace rm{
    GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
    GLuint loadBMP(const char* imagepath);
    bool loadOBJ(const char * path,
        std::vector < glm::vec3 > & out_vertices,
        std::vector < glm::vec2 > & out_uvs,
        std::vector < glm::vec3 > & out_normals
    );
}
