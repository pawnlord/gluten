#ifndef OBJECT_GENERATOR_H
#define OBJECT_GENERATOR_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "GraphicsManager.h"

namespace gluten {

struct ObjectGenerator {
    // These are used somewhere, but not in the current pipeline
    // TODO: figure out good way to generate faces with these
    // (I think it only maters if we don't know the normal of the face? not sure though)
    // std::vector<vec3> normals;
    std::vector<vec3> vertices;
    std::vector<vec3> colors;
    std::shared_ptr<ShaderPipeline> pipeline;

    ObjectGenerator *addTri(
        glm::vec3 p1,
        glm::vec3 p2,
        glm::vec3 p3,
        glm::vec3 color
    );

    ObjectGenerator *addQuad(
        glm::vec3 p1,
        glm::vec3 p2,
        glm::vec3 p3,
        glm::vec3 p4,
        glm::vec3 color
    );
    
    ObjectGenerator *addPipeline(std::shared_ptr<ShaderPipeline> pipeline);
    
    NonUVObject *generate();
};

} // namespace gluten

#endif // OBJECT_GENERATOR_H