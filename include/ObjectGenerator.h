#ifndef OBJECT_GENERATOR_H
#define OBJECT_GENERATOR_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "GraphicsManager.h"

namespace gluten {

struct ObjectInfo {
    std::vector<vec3> normals;
    std::vector<vec3> vertices;
    std::vector<vec3> colors;
    

    ObjectInfo *addTri(
        glm::vec3 p1,
        glm::vec3 p2,
        glm::vec3 p3,
        glm::vec3 color
    );

    ObjectInfo *addQuad(
        glm::vec3 p1,
        glm::vec3 p2,
        glm::vec3 p3,
        glm::vec3 p4,
        glm::vec3 color
    );
    
    NonUVObject generate();
};

} // namespace gluten

#endif // OBJECT_GENERATOR_H