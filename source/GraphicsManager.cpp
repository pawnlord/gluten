#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

#include "../include/GraphicsManager.h"
#include "../include/ResourceManager.h"

namespace gm {
    int width = 0, height = 0;
    
    GLuint setupGraphicsManager(GLFWwindow* window, glm::mat4& Projection, float projectionAngle){
		glfwGetWindowSize(window, &width, &height);
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(glm::radians(projectionAngle), (float) width / (float)height, 0.1f, 100.0f);
        return 1;
    }

    GLuint drawColoredArray(int attribLocation, int attribSize, 
                        GltnGraphicsContext ctx, glm::mat4 Model,
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size){
        
        glm::mat4 mvp = ctx.projection * ctx.view * Model; // Remember, matrix multiplication is the other way around

        		
		// Get a handle for our "MVP" uniform
		GLuint matrixID = glGetUniformLocation(programID, "MVP");
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

		GLuint brightnessID = glGetUniformLocation(programID, "brightness_scalar");
		
		glUniform1f(brightnessID, ctx.brightnessScalar);		
        
		// 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
		glVertexAttribPointer(attribLocation, attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		
		glVertexAttribPointer(attribLocation+1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glUseProgram(programID);
        
		glDrawArrays(GL_TRIANGLES, 0, buffer_size); // Starting from vertex 0; 3 vertices total -> 1 triangle
        return 1;
    }
    GLuint drawTexturedArray(int attribLocation, int attribSize, 
                        GltnGraphicsContext ctx, glm::mat4 Model,
                        GLuint programID, GLuint vertexbuffer, GLuint uvbuffer,
                        int buffer_size){
        
        glm::mat4 mvp = ctx.projection * ctx.view * Model; // Remember, matrix multiplication is the other way around


		// Get a handle for our "MVP" uniform
		GLuint matrixID = glGetUniformLocation(programID, "MVP");
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

		GLuint brightnessID = glGetUniformLocation(programID, "bs");
		
		glUniform1f(brightnessID, ctx.brightnessScalar);		
		
		// 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
		glVertexAttribPointer(attribLocation, attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		
		glVertexAttribPointer(attribLocation+1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glUseProgram(programID);
        
		glDrawArrays(GL_TRIANGLES, 0, buffer_size); // Starting from vertex 0; 3 vertices total -> 1 triangle
        return 1;
    }

    std::vector<GLfloat> unpack_vec3_vector(std::vector<vec3> v){
        std::vector<GLfloat> arr;
        for(int i = 0; i < v.size(); i++){
            arr.push_back(v[i].x);
            arr.push_back(v[i].y);
            arr.push_back(v[i].z);
        }
        return arr;
    }
    std::vector<GLfloat> unpack_vec2_vector(std::vector<vec2> v){
        std::vector<GLfloat> arr;
        for(int i = 0; i < v.size(); i++){
            arr.push_back(v[i].x);
            arr.push_back(v[i].y);
        }
        return arr;
    }
    GLuint genBuffer2(GLuint* buffer, GLuint buffer_size, std::vector<vec2> v) {
        return genBuffer(buffer, buffer_size, unpack_vec2_vector(v).data());
    }
    GLuint genBuffer3(GLuint* buffer, GLuint buffer_size, std::vector<vec3> v) {
        return genBuffer(buffer, buffer_size, unpack_vec3_vector(v).data());
    }
    
    GLuint genBuffer(GLuint* buffer, GLuint buffer_size, GLfloat* buffer_data) {
        glGenBuffers(1, buffer);
        glBindBuffer(GL_ARRAY_BUFFER, *buffer);
        glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW);
        return 1;
    }
    GLuint loadTexture(GLuint textureID) {
        glBindTexture(GL_TEXTURE_2D, textureID);

        return 1;
    }

    void GltnInternal::updateModel(std::function<void(glm::mat4& model)> fp){
        fp(model);
    }

    void GltnInternal::display(GltnGraphicsContext ctx) {
        mvp = ctx.projection * ctx.view * model; // Remember, matrix multiplication is the other way around
        pipeline->draw(ctx, model, vertices.size());
    };
    GltnInternal *GltnInternal::addBrightnessScalar(std::string name){
        pipeline->addUniformVariable(shader::Float1, name, &brightnessScalar);
        return this;
    }
    GltnInternal *GltnInternal::addMVP(std::string name){
        pipeline->addUniformVariable(shader::Mat4, name, &mvp);
        return this;
    }
    GltnInternal *GltnInternal::addVertexBuffer(int attribNum){
        return addInShaderVariable(attribNum, 3, vertexbuffer);
    }
    GltnInternal *GltnInternal::addUniformVariable(shader::ShaderInputType type, std::string name, void *data){
        pipeline->addUniformVariable(type, name, data);
        return this;
    }
    GltnInternal *GltnInternal::addInShaderVariable(int attribNum, int attribSize, GLuint& buffer){
        pipeline->addInShaderVariable(attribNum, attribSize, buffer);
        return this;
    }
    

    void GltnUVObject::load(std::string path){
        rm::loadObjWithUV(path.c_str(), internals.vertices, uvs, internals.normals);  
        gm::genBuffer3(&internals.vertexbuffer, internals.vertices.size()*6*3, internals.vertices); // TODO: why *4?
        gm::genBuffer2(&uvbuffer, internals.vertices.size()*6*3, uvs); // TODO: why *4?

    }

    void GltnNonUVObject::load(std::string path){
        rm::loadObjWithoutUV(path.c_str(), internals.vertices, internals.normals);  
        gm::genBuffer3(&internals.vertexbuffer, internals.vertices.size()*6*3, internals.vertices); // TODO: why *4?
        for(auto v : internals.vertices){
            colors.push_back(glm::vec3(1.0, 0, 1.0));
        }
        gm::genBuffer3(&colorbuffer, colors.size()*6*3, colors); // TODO: why *4?
    }

    void GltnNonUVObject::display(GltnGraphicsContext ctx) {
        internals.display(ctx);
    }

    void GltnUVObject::display(GltnGraphicsContext ctx) {
        if(textureID != 0){
    		loadTexture(textureID);
        }
        internals.display(ctx);
    }
    
    void GltnUVObject::updateModel(std::function<void(glm::mat4& model)> fp){
        internals.updateModel(fp);
    }
    void GltnUVObject::usingTexture(std::string path){
    	textureID = rm::loadBMP(path.c_str());
    }
    
}