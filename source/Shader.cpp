#include "../include/Shader.h"

using namespace shader;

GltnShaderPipeline *GltnShaderPipeline::addUniformVariable(ShaderInputType type, std::string name, void *data){
    uniforms.push_back(UniformShaderInput{type, name, data});
    return this;
}

GltnShaderPipeline *GltnShaderPipeline::addInShaderVariable(int attribNum, int attribSize, GLuint& buffer){
    vertexInputs.push_back(VertexShaderInput{attribNum, attribSize, buffer});
    return this;
}

void GltnShaderPipeline::draw(gm::GltnGraphicsContext ctx, glm::mat4 Model, GLuint programID){
    for(auto& uniform : this->uniforms){
        GLuint id = glGetUniformLocation(programID, uniform.name.c_str());
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
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexInput.buffer);
        
		glVertexAttribPointer(attribLocation+vertexInput.attribNum, vertexInput.attribSize, 
                GL_FLOAT, GL_FALSE, 
                0, (void*)0);
		
    }
    glUseProgram(programID);
    glDrawArrays(GL_TRIANGLES, 0, verticesNum); 
}
