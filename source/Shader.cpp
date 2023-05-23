
#define GLEW_STATIC

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

using namespace shader;

GltnShaderPipeline *GltnShaderPipeline::addUniformVariable(ShaderInputType type, std::string name, void *data){
    uniforms.push_back(UniformShaderInput{type, name, data});
    return this;
}

GltnShaderPipeline *GltnShaderPipeline::addInShaderVariable(int attribNum, int attribSize, GLuint& buffer){
    vertexInputs.push_back(VertexShaderInput{attribNum, attribSize, buffer});
    return this;
}

void GltnShaderPipeline::draw(gm::GltnGraphicsContext ctx, glm::mat4 Model, int verticesNum){
    for(auto& uniform : this->uniforms){
        GLuint id = glGetUniformLocation(shaderID, uniform.name.c_str());
        // stupid bit-hack
        if(id == ~0){
            std::stringstream message;
            message << "Inavlid Uniform variable in shader " << shaderID << ": Was unable to find uniform " << uniform.name; 
            throw std::invalid_argument(message.str());
        }
	    switch(uniform.type){
            case Mat4:
            glUniformMatrix4fv(id, 1, GL_FALSE, (GLfloat*)uniform.data);
            break;
            case Float1:
            glUniform1f(id,  *(GLfloat*)uniform.data);
            break;
            case Vec3:
            glUniform3fv(id, 1, (GLfloat*)uniform.data);
            break;
        }
    }
    for(auto& vertexInput : this->vertexInputs){
        glEnableVertexAttribArray(vertexInput.attribNum);
        glBindBuffer(GL_ARRAY_BUFFER, vertexInput.buffer);
        
		glVertexAttribPointer(attribLocation+vertexInput.attribNum, vertexInput.attribSize, 
                GL_FLOAT, GL_FALSE, 
                0, (void*)0);
		
    }
    glUseProgram(shaderID);
    glDrawArrays(GL_TRIANGLES, 0, verticesNum*3); 
    // glm::mat4 mvp = ctx.projection * ctx.view * Model; // Remember, matrix multiplication is the other way around


    // Get a handle for our "MVP" uniform
    // GLuint matrixID = glGetUniformLocation(programID, "MVP");
    
    // // Send our transformation to the currently bound shader, in the "MVP" uniform
    // glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    // GLuint brightnessID = glGetUniformLocation(programID, "bs");
    
    // glUniform1f(brightnessID, ctx.brightnessScalar);		
    
    // // 1st attribute buffer : vertices
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexInputs[0].buffer);
    
    // glVertexAttribPointer(attribLocation, vertexInputs[0].attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // // 2nd attribute buffer : colors
    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexInputs[1].buffer);
    
    // glVertexAttribPointer(attribLocation+1, vertexInputs[1].attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // glUseProgram(programID);
    
    // glDrawArrays(GL_TRIANGLES, 0, verticesNum*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        
}
