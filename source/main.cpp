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

using namespace gluten;

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

    GraphicsContext ctx{1024, 768, "Tutorial 01", 45.0};
        
    GLuint vertexbuffer;
	genBuffer3(&vertexbuffer, (GLuint)g_vertex_buffer_data.size() * 6 * 3, g_vertex_buffer_data); // TODO: why *4?
	GLuint uvbuffer;
	genBuffer2(&uvbuffer, (GLuint)g_uv_buffer_data.size() * 6 * 2, g_uv_buffer_data);

    ShaderPipeline uvPipeline{
        SHADERS_FOLDER "uvvertexshader.glsl", 
        SHADERS_FOLDER "uvfragmentshader.glsl", 
        0};
        
    ShaderPipeline colorPipeline{
        SHADERS_FOLDER "colorvertexshader.glsl", 
        SHADERS_FOLDER "colorfragmentshader.glsl", 
        0};
    
    uvPipeline.addUniformVariable(Float1, "bs")
    ->addUniformVariable(Mat4, "MVP")
    ->addInShaderVariable(0, 3)
    ->addInShaderVariable(1, 2);
    
    colorPipeline.addUniformVariable(Float1, "bs")
    ->addUniformVariable(Mat4, "MVP")
    ->addInShaderVariable(0, 3)
    ->addInShaderVariable(1, 3);


    UVObject skewedCube{RES_FOLDER "test_obj.obj", std::shared_ptr<ShaderPipeline>(&uvPipeline)};
    
    skewedCube.internals.addBrightnessScalar("bs")
    ->addMVP("MVP");

    skewedCube.internals.addVertexBuffer()
    ->addInShaderVariable(&skewedCube.uvbuffer);

    skewedCube.usingTexture(RES_FOLDER "textureExample.bmp");

    NonUVObject monkey{RES_FOLDER "test2.obj", std::shared_ptr<ShaderPipeline>(&colorPipeline)};

    monkey.internals.addBrightnessScalar("bs")
    ->addMVP("MVP");

    monkey.internals.addVertexBuffer()
    ->addInShaderVariable(&monkey.colorbuffer);


	GLfloat t = 0;
    
    skewedCube.internals.addUpdate([&](auto& model){
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0, glm::sin(t) * 3,  0));
    });

	GLuint textureID = rm::loadBMP(RES_FOLDER "textureExample.bmp");
	GLuint textureID2 = rm::loadBMP(RES_FOLDER "textureExample2.bmp");

    InputInfo inputInfo;
    Camera camera;

    registerInputInfo(inputInfo);

    inputInfo.setBrightnessScalar(&skewedCube.internals.brightnessScalar);
	
    glfwSetScrollCallback(ctx.window, scrollCallback);
    
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    do{
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		computeMatricesFromInputs(ctx.window, camera, inputInfo);
		
		ctx.view = camera.getViewMatrix();
		ctx.projection = camera.getProjectionMatrix();
        		
		glm::mat4 model = glm::mat4(1.0f);
		loadTexture(textureID);

        skewedCube.display(ctx);

		glDisableVertexAttribArray(0);
        
        glfwSwapBuffers(ctx.window);
        glfwPollEvents();
		t += (GLfloat)0.0005;
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(ctx.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(ctx.window) == 0 );
}