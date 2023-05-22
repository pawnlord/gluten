// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#define GLEW_STATIC

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
    GLuint programID = rm::LoadShaders( "source/shaders/vertexshader.glsl", "source/shaders/fragmentshader.glsl" );
        
    GLuint vertexbuffer;
	gm::genBuffer3(&vertexbuffer, g_vertex_buffer_data.size()*6*3, g_vertex_buffer_data); // TODO: why *4?
	GLuint uvbuffer;
	gm::genBuffer2(&uvbuffer, g_uv_buffer_data.size()*6*2, g_uv_buffer_data);

    gm::GltnFileObject monkey{"test_obj.obj"};
    monkey.usingTexture("textureExample2.bmp");
	GLfloat t = 0;
	GLuint textureID = rm::loadBMP("textureExample.bmp");
	GLuint textureID2 = rm::loadBMP("textureExample2.bmp");
	
    im::setBrightnessScalar(&ctx.brightnessScalar);
	glfwSetScrollCallback(ctx.window, im::scrollCallback);

    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Get width and height
		
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
		
		im::computeMatricesFromInputs(ctx.window);
		
		// Camera matrix
		ctx.view = im::getViewMatrix();
		ctx.projection = im::getProjectionMatrix();
        
		
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 model = glm::mat4(1.0f);
		gm::loadTexture(textureID);

		// model = glm::rotate(model, glm::radians(t), glm::vec3(1, 1, 0)); 
		// gm::drawTexturedArray(0, 3, 
		// 	ctx, model,
		// 	programID,
		// 	vertexbuffer, 
		// 	uvbuffer,
		// 	12*3
		// );

        monkey.updateModel([=](auto& model){
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(glm::sin(t) * 3, 0, 0));
        });
        monkey.display(ctx, programID);

		glDisableVertexAttribArray(0);
        
		// Swap buffers
        glfwSwapBuffers(ctx.window);
        glfwPollEvents();
		t+=0.0005;
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(ctx.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(ctx.window) == 0 );
}