
#ifndef GLTNSHADER_H
#define GLTNSHADER_H
// File for describing the behavior and layout of a shader pipeline
#include <memory>
#include <string>
#include <vector>
#define GLEW_STATIC
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
    void *data;
    UniformShaderInput(ShaderInputType type, std::string name, void *data) : type{type}, name{name}, data{data} {}
};

struct VertexShaderInput {
    int attribNum;
    int attribSize;
    GLuint& buffer;
    VertexShaderInput(int attribNum, int attribSize, GLuint& buffer) : attribNum{attribNum}, attribSize{attribSize}, buffer{buffer} {}
};

class GltnShaderPipeline;

class GltnShaderPipeline{
    public:
    int attribLocation;
    int verticesNum;
    std::vector<UniformShaderInput> uniforms; // Inputs uniform accross shader "instances?"
    std::vector<VertexShaderInput> vertexInputs;  // Inputs with buffers for vertex shaders
    GLuint shaderID;
    GltnShaderPipeline(std::string vertexShader, std::string fragmentShader, int attribLocation, int verticesNum) 
        : attribLocation{attribLocation}, verticesNum{verticesNum} {
        // TODO: Cache Shaders
        shaderID = rm::LoadShaders(vertexShader.c_str(), fragmentShader.c_str());
    }

    GltnShaderPipeline *addUniformVariable(ShaderInputType type, std::string name, void *data);
    GltnShaderPipeline *addInShaderVariable(int attribNum, int attribSize, GLuint& buffer);
    void draw(gm::GltnGraphicsContext ctx, glm::mat4 Model);

};

} // namespace shader
#endif