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

static  GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static GLfloat tri_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

static GLfloat tri_color_buffer_data[] = {
   1.0f, 1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f,  1.0f, 1.0f,
};

int main(){
	static GLfloat g_color_buffer_data[12*3*3];
	for (int v = 0; v < 12*3 ; v++){
		g_color_buffer_data[3*v+0] = g_vertex_buffer_data[3*v+0];
		g_color_buffer_data[3*v+1] = g_vertex_buffer_data[3*v+1];
		g_color_buffer_data[3*v+2] = g_vertex_buffer_data[3*v+2];
	}
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

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	
    // This will identify our vertex buffer
    GLuint tri_vertexbuffer;
    glGenBuffers(1, &tri_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertex_buffer_data), tri_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint tri_colorbuffer;
	glGenBuffers(1, &tri_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tri_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_color_buffer_data), tri_color_buffer_data, GL_STATIC_DRAW);
	
	glm::mat4 Projection;
	gm::setupGraphicsManager(window, Projection, 45.0f);
	GLfloat t = 0;
    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Get width and height
		
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
		
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(4,4,4),
			glm::vec3(0,0,0), 
			glm::vec3(0,1,0) 
		);
		
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		
		Model = glm::rotate(Model, glm::radians(t), glm::vec3(1, 1, 0)); 
		
		gm::drawArray(0, 3, 
			Projection, View, Model,
		 	g_vertex_buffer_data, 
			g_color_buffer_data, 
			programID,
			vertexbuffer, 
			colorbuffer,
			12*3
		);
		Model = glm::translate(Model, glm::vec3(0, 0, 1.001));
		gm::drawArray(0, 3, 
			Projection, View, Model,
		 	tri_vertex_buffer_data, 
			tri_color_buffer_data, 
			programID,
			tri_vertexbuffer, 
			tri_colorbuffer,
			3*3
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