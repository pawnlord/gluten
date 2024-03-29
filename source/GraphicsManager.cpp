#include "GraphicsManager.h"

// #define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

#include "ResourceManager.h"

namespace gluten {

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

    void ObjectInternal::updateModel(std::function<void(glm::mat4& model)> fp){
        fp(model);
    }

    void ObjectInternal::display(GraphicsContext ctx) {
        for (auto& update : this->updates) {
            this->model = update(this->model);
        }
        this->mvp = ctx.projection * ctx.view * this->model; 
        pipeline->draw(ctx, model, (GLuint)vertices.size(), uniformValues, this->buffers);
    }

    ObjectInternal *ObjectInternal::addBrightnessScalar(std::string name){
        return addUniformVariable(name, &brightnessScalar);
    }

    ObjectInternal *ObjectInternal::addMVP(std::string name){
        return addUniformVariable(name, &this->mvp);
    }

    ObjectInternal *ObjectInternal::addVertexBuffer(){
        return addInShaderVariable(&vertexbuffer);
    }

    ObjectInternal *ObjectInternal::addUniformVariable(std::string name, void *data){
        uniformValues[name] = data;
        return this;
    }

    ObjectInternal *ObjectInternal::addInShaderVariable(GLuint *buffer){
        this->buffers.push_back(buffer);
        return this;
    }

    ObjectInternal *ObjectInternal::addUpdate(std::function<glm::mat4(glm::mat4 model)> fp) {
        updates.push_back(fp);
        return this;
    }


    void UVObject::load(std::string path){
        rm::loadObjWithUV(path.c_str(), internals.vertices, uvs, internals.normals);  
        gluten::genBuffer3(&internals.vertexbuffer, internals.vertices.size()*4*3, internals.vertices);
        gluten::genBuffer2(&uvbuffer, internals.vertices.size()*4*3, uvs);
    }

    void UVObject::display(GraphicsContext ctx) {
        if(textureID != 0){
    		loadTexture(textureID);
        }
        internals.display(ctx);
    }
    
    void UVObject::updateModel(std::function<void(glm::mat4& model)> fp){
        internals.updateModel(fp);
    }

    void UVObject::usingTexture(std::string path){
    	if (path == ""){
            return;
        }
        textureID = rm::loadBMP(path);
    }
    
    void NonUVObject::load(std::string path){
        rm::loadObjWithoutUV(path.c_str(), internals.vertices, internals.normals);  
        for(auto v : internals.vertices){
            colors.push_back(glm::vec3(1.0, 1.0, 1.0));
        }
        genBuffers();
    }

    void NonUVObject::display(GraphicsContext ctx) {
        internals.display(ctx);
    }
    
}