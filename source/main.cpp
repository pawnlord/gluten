// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
// #define GLEW_STATIC

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>
using namespace glm;

#include "../include/ResourceManager.h"
#include "../include/GraphicsManager.h"
#include "../include/InputManager.h"
#include "../include/Shader.h"

#include "config.h"

static  std::vector<vec3> g_vertex_buffer_data = {
    vec3(-1.0f,-1.0f,-1.0f), // triangle 1 : begin
    vec3(-1.0f,-1.0f, 1.0f),
    vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
    vec3(1.0f, 1.0f,-1.0f), // triangle 2 : begin
    vec3(-1.0f,-1.0f,-1.0f),
    vec3(-1.0f, 1.0f,-1.0f), // triangle 2 : end
    vec3(1.0f,-1.0f, 1.0f),
    vec3(-1.0f,-1.0f,-1.0f),
    vec3(1.0f,-1.0f,-1.0f),
    vec3(1.0f, 1.0f,-1.0f),
    vec3(1.0f,-1.0f,-1.0f),
    vec3(-1.0f,-1.0f,-1.0f),
    vec3(-1.0f,-1.0f,-1.0f),
    vec3(-1.0f, 1.0f, 1.0f),
    vec3(-1.0f, 1.0f,-1.0f),
    vec3(1.0f,-1.0f, 1.0f),
    vec3(-1.0f,-1.0f, 1.0f),
    vec3(-1.0f,-1.0f,-1.0f),
    vec3(-1.0f, 1.0f, 1.0f),
    vec3(-1.0f,-1.0f, 1.0f),
    vec3(1.0f,-1.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f,-1.0f,-1.0f),
    vec3(1.0f, 1.0f,-1.0f),
    vec3(1.0f,-1.0f,-1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f,-1.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 1.0f,-1.0f),
    vec3(-1.0f, 1.0f,-1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(-1.0f, 1.0f,-1.0f),
    vec3(-1.0f, 1.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(-1.0f, 1.0f, 1.0f),
    vec3(1.0f,-1.0f, 1.0f)
};

// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
static std::vector<vec2> g_uv_buffer_data = {
    vec2(0.000059f, 1.0f-0.000004f),
    vec2(0.000103f, 1.0f-0.336048f),
    vec2(0.335973f, 1.0f-0.335903f),
    vec2(1.000023f, 1.0f-0.000013f),
    vec2(0.667979f, 1.0f-0.335851f),
    vec2(0.999958f, 1.0f-0.336064f),
    vec2(0.667979f, 1.0f-0.335851f),
    vec2(0.336024f, 1.0f-0.671877f),
    vec2(0.667969f, 1.0f-0.671889f),
    vec2(1.000023f, 1.0f-0.000013f),
    vec2(0.668104f, 1.0f-0.000013f),
    vec2(0.667979f, 1.0f-0.335851f),
    vec2(0.000059f, 1.0f-0.000004f),
    vec2(0.335973f, 1.0f-0.335903f),
    vec2(0.336098f, 1.0f-0.000071f),
    vec2(0.667979f, 1.0f-0.335851f),
    vec2(0.335973f, 1.0f-0.335903f),
    vec2(0.336024f, 1.0f-0.671877f),
    vec2(1.000004f, 1.0f-0.671847f),
    vec2(0.999958f, 1.0f-0.336064f),
    vec2(0.667979f, 1.0f-0.335851f),
    vec2(0.668104f, 1.0f-0.000013f),
    vec2(0.335973f, 1.0f-0.335903f),
    vec2(0.667979f, 1.0f-0.335851f),
    vec2(0.335973f, 1.0f-0.335903f),
    vec2(0.668104f, 1.0f-0.000013f),
    vec2(0.336098f, 1.0f-0.000071f),
    vec2(0.000103f, 1.0f-0.336048f),
    vec2(0.000004f, 1.0f-0.671870f),
    vec2(0.336024f, 1.0f-0.671877f),
    vec2(0.000103f, 1.0f-0.336048f),
    vec2(0.336024f, 1.0f-0.671877f),
    vec2(0.335973f, 1.0f-0.335903f),
    vec2(0.667969f, 1.0f-0.671889f),
    vec2(1.000004f, 1.0f-0.671847f),
    vec2(0.667979f, 1.0f-0.335851f)
};


int main(){

    gm::GltnGraphicsContext ctx{1024, 768, "Tutorial 01", 45.0};
        
    GLuint vertexbuffer;
	gm::genBuffer3(&vertexbuffer, g_vertex_buffer_data.size()*6*3, g_vertex_buffer_data); // TODO: why *4?
	GLuint uvbuffer;
	gm::genBuffer2(&uvbuffer, g_uv_buffer_data.size()*6*2, g_uv_buffer_data);

    shader::GltnShaderPipeline uvPipeline{
        SHADERS_FOLDER "uvvertexshader.glsl", 
        SHADERS_FOLDER "uvfragmentshader.glsl", 
        0};
        
    shader::GltnShaderPipeline colorPipeline{
        SHADERS_FOLDER "colorvertexshader.glsl", 
        SHADERS_FOLDER "colorfragmentshader.glsl", 
        0};
    
    uvPipeline.addUniformVariable(shader::Float1, "bs")
    ->addUniformVariable(shader::Mat4, "MVP")
    ->addInShaderVariable(0, 3)
    ->addInShaderVariable(1, 2);
    
    colorPipeline.addUniformVariable(shader::Float1, "bs")
    ->addUniformVariable(shader::Mat4, "MVP")
    ->addInShaderVariable(0, 3)
    ->addInShaderVariable(1, 3);


    gm::GltnUVObject skewedCube{RES_FOLDER "test_obj.obj", std::shared_ptr<shader::GltnShaderPipeline>(&uvPipeline)};
    
    skewedCube.internals.addBrightnessScalar("bs")
    ->addMVP("MVP");

    skewedCube.internals.addVertexBuffer()
    ->addInShaderVariable(&skewedCube.uvbuffer);

    skewedCube.usingTexture(RES_FOLDER "textureExample2.bmp");

    gm::GltnNonUVObject monkey{RES_FOLDER "test2.obj", std::shared_ptr<shader::GltnShaderPipeline>(&colorPipeline)};

    monkey.internals.addBrightnessScalar("bs")
    ->addMVP("MVP");

    monkey.internals.addVertexBuffer()
    ->addInShaderVariable(&monkey.colorbuffer);


	GLfloat t = 0;
	GLuint textureID = rm::loadBMP("textureExample.bmp");
	GLuint textureID2 = rm::loadBMP("textureExample2.bmp");
	
    im::setBrightnessScalar(&skewedCube.internals.brightnessScalar);
	glfwSetScrollCallback(ctx.window, im::scrollCallback);

    do{
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		im::computeMatricesFromInputs(ctx.window);
		
		ctx.view = im::getViewMatrix();
		ctx.projection = im::getProjectionMatrix();
        		
		glm::mat4 model = glm::mat4(1.0f);
		gm::loadTexture(textureID);

        skewedCube.updateModel([=](auto& model){
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(glm::sin(t) * 3, 0, 0));
        });
        skewedCube.display(ctx);
        
        monkey.updateModel([=](auto& model){
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0, glm::sin(t) * 3, 0));
        });
        monkey.display(ctx);

		glDisableVertexAttribArray(0);
        
        glfwSwapBuffers(ctx.window);
        glfwPollEvents();
		t+=0.0005;
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(ctx.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(ctx.window) == 0 );
}