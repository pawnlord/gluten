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
    GltnGraphicsContext::GltnGraphicsContext(int width, int height, std::string name, float projectionAngle){
        // Initialise GLFW
        if( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            exit(-1);
        }
        glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

        // Open a window and create its OpenGL context
        window = glfwCreateWindow( width, height, name.c_str(), NULL, NULL);
        if( window == NULL ){
            fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window); // Initialize GLEW
        glewExperimental=GL_TRUE; // Needed in core profile
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            exit(-1);
        }
        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        /* SETUP COMPLETE
        */

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);


		glfwGetWindowSize(window, &width, &height);
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		projection = glm::perspective(glm::radians(projectionAngle), (float) width / (float)height, 0.1f, 100.0f);
        
        glfwSetCursorPos(window, width/2, height/2);
        
    }
    
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



    void GltnFileObject::load(std::string path){
        rm::loadOBJ(path.c_str(), vertices, uvs, normals);  
        gm::genBuffer3(&vertexbuffer, vertices.size()*6*3, vertices); // TODO: why *4?
        gm::genBuffer2(&uvbuffer, vertices.size()*6*3, uvs); // TODO: why *4?

    }

    void GltnFileObject::display(GltnGraphicsContext ctx, GLuint shaderID) {
        t += 0.01;
        if(textureID != 0){
    		loadTexture(textureID);
        }
        pipeline.draw(ctx, model, shaderID);
    }
    
    void GltnFileObject::updateModel(std::function<void(glm::mat4& model)> fp){
        fp(model);
    }
    void GltnFileObject::usingTexture(std::string path){
    	textureID = rm::loadBMP(path.c_str());
    }
    
}