
#ifndef GLTNSHADER_H
#define GLTNSHADER_H
// File for describing the behavior and layout of a shader pipeline
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
// #define GLEW_STATIC
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

#include "../include/GraphicsContext.h"
#include "../include/ResourceManager.h"

namespace shader{

enum ShaderInputType {
    Vec3, Mat4, Float1
};

struct UniformShaderInput{
    ShaderInputType type;
    std::string name;
    UniformShaderInput(ShaderInputType type, std::string name) : type{type}, name{name} {}
};

struct VertexShaderInput {
    int attribNum;
    int attribSize;
    VertexShaderInput(int attribNum, int attribSize) : attribNum{attribNum}, attribSize{attribSize} {}
};

class GltnShaderPipeline;

class GltnShaderPipeline{
    public:
    int attribLocation;
    std::vector<UniformShaderInput> uniforms; // Inputs uniform accross shader "instances?"
    std::vector<VertexShaderInput> vertexInputs;  // Inputs with buffers for vertex shaders
    GLuint shaderID;
    GltnShaderPipeline(std::string vertexShader, std::string fragmentShader, int attribLocation) 
        : attribLocation{attribLocation} {
        // TODO: Cache Shaders
        shaderID = rm::LoadShaders(vertexShader.c_str(), fragmentShader.c_str());
    }

    GltnShaderPipeline *addUniformVariable(ShaderInputType type, std::string name);
    GltnShaderPipeline *addInShaderVariable(int attribNum, int attribSize);
    void draw(gm::GltnGraphicsContext ctx, 
        glm::mat4 Model, 
        int verticesNum, 
        const std::unordered_map<std::string, void *>& uniformValues, 
        const std::vector<GLuint*>& buffers); 

};

} // namespace shader
#endif