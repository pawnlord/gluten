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


int main(){
   // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=GL_TRUE; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    GLuint programID = rm::LoadShaders( "source/shaders/vertexshader.glsl", "source/shaders/fragmentshader.glsl" );
    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
    /* SETUP COMPLETE
     */

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    std::vector<vec3> normals_buffer_data;
    rm::loadOBJ("test_obj.obj", g_vertex_buffer_data, g_uv_buffer_data, normals_buffer_data);

    // This will identify our vertex buffer
    GLuint vertexbuffer;
	gm::genBuffer(&vertexbuffer, g_vertex_buffer_data.size()*6*3, unpack_vec3_vector(g_vertex_buffer_data).data()); // TODO: why *4?
	GLuint uvbuffer;
	gm::genBuffer(&uvbuffer, g_uv_buffer_data.size()*6*2, unpack_vec2_vector(g_uv_buffer_data).data());
	
	glm::mat4 Projection;
	gm::setupGraphicsManager(window, Projection, 45.0f);
	GLfloat t = 0;
	GLuint textureID = rm::loadBMP("textureExample.bmp");
	GLuint textureID2 = rm::loadBMP("textureExample2.bmp");
	
	glfwSetScrollCallback(window, im::scrollCallback);

    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Get width and height
		
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
		
		im::computeMatricesFromInputs(window);
		
		// Camera matrix
		glm::mat4 View = im::getViewMatrix();
		Projection = im::getProjectionMatrix();
		
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		gm::loadTexture(textureID);
		Model = glm::rotate(Model, glm::radians(t), glm::vec3(1, 1, 0)); 
		gm::drawTexturedArray(0, 3, 
			Projection, View, Model,
			programID,
			vertexbuffer, 
			uvbuffer,
			12*3
		);

		gm::loadTexture(textureID2);
		Model = glm::rotate(Model, glm::radians(-t), glm::vec3(1, 1, 0)); 
	
		gm::drawTexturedArray(0, 3, 
			Projection, View, Model,
			programID,
			vertexbuffer, 
			uvbuffer,
			12*3
		);

		glDisableVertexAttribArray(0);
        
		// Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
		t+=0.1;
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );
}