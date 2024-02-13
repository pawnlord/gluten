#include "ObjectGenerator.h"

namespace gluten {


ObjectGenerator *ObjectGenerator::addTri(
    glm::vec3 p1,
    glm::vec3 p2,
    glm::vec3 p3,
    glm::vec3 color
) {
    vertices.push_back(p1);
    colors.push_back(color);
    vertices.push_back(p2);
    colors.push_back(color);
    vertices.push_back(p3);
    colors.push_back(color);
    return this;
}

ObjectGenerator *ObjectGenerator::addPipeline(std::shared_ptr<ShaderPipeline> pipeline) {
    this->pipeline = pipeline;
    return this;
}

std::shared_ptr<NonUVObject> ObjectGenerator::generate() {
    ObjectInternal internals{pipeline};
    internals.vertices = vertices;
    
    std::shared_ptr<NonUVObject> object = std::make_shared<NonUVObject>(internals, colors);

    // TODO: This is making assumptions about our shader pipeline
    // Best to change this later
    object->internals.addBrightnessScalar("bs")
        ->addMVP("MVP")
        ->addVertexBuffer()
        ->addInShaderVariable(&object->colorbuffer);
    
    return object;
}


} // namespace gluten