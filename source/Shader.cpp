
// #define GLEW_STATIC

#include "../include/Shader.h"

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>
namespace gluten {

ShaderPipeline *ShaderPipeline::addUniformVariable(ShaderInputType type, std::string name){
    uniforms.push_back(UniformShaderInput{type, name});
    return this;
}

ShaderPipeline *ShaderPipeline::addInShaderVariable(int attribNum, int attribSize){
    vertexInputs.push_back(VertexShaderInput{attribNum, attribSize});
    return this;
}

void ShaderPipeline::draw(gluten::GraphicsContext ctx, 
        glm::mat4 model, 
        int verticesNum, 
        const std::unordered_map<std::string, void *>& uniformValues, 
        const std::vector<GLuint*>& buffers) {

    glUseProgram(this->shaderID);
    if(buffers.size() != this->vertexInputs.size()){
        std::stringstream message;
        message << "Invalid number of buffers for " << this->shaderID << ": " << this->vertexInputs.size() << " vertex inputs != " << buffers.size() << " buffers"; 
        throw std::invalid_argument(message.str());
    }

    for(const auto& uniform : this->uniforms){
        GLuint id = glGetUniformLocation(this->shaderID, uniform.name.c_str());

        if(id == ~0){
            std::stringstream message;
            message << "Inavlid Uniform variable in shader " << this->shaderID << ": Was unable to find uniform " << uniform.name; 
            throw std::invalid_argument(message.str());
        }

        void *data = uniformValues.at(uniform.name);
	    switch(uniform.type){
            case Mat4:
            glUniformMatrix4fv(id, 1, GL_FALSE, (GLfloat*)data);
            break;
            case Float1:
            glUniform1f(id,  *(GLfloat*)data);
            break;
            case Vec3:
            glUniform3fv(id, 1, (GLfloat*)data);
            break;
        }
    }

    for(const auto& vertexInput : this->vertexInputs){
        glEnableVertexAttribArray(vertexInput.attribNum);
        // assumptions that need to be checked: attrib nums are increasing, and easy to put in order
         // I don't like this solution
        GLuint buffer = *(buffers[vertexInput.attribNum]);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
		glVertexAttribPointer(attribLocation+vertexInput.attribNum, vertexInput.attribSize, 
                GL_FLOAT, GL_FALSE, 
                0, (void*)0);
		
    }
    glDrawArrays(GL_TRIANGLES, 0, verticesNum*3); 
        
}

}