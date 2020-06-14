#define GLEW_STATIC
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>

#include "../include/GraphicsManager.h"

namespace gm {
    int width = 0, height = 0;
    GLuint setupGraphicsManager(GLFWwindow* window, glm::mat4& Projection, float projectionAngle){
		glfwGetWindowSize(window, &width, &height);
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(glm::radians(projectionAngle), (float) width / (float)height, 0.1f, 100.0f);
        return 1;
    }
    GLuint drawArray(int attribLocation, int attribSize, 
                        glm::mat4 Projection,  glm::mat4 View, glm::mat4 Model,
                        GLfloat g_vertex_buffer_data[], GLfloat g_color_buffer_data[],
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size){
        
        glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
		
		// Get a handle for our "MVP" uniform
		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
		// 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
		glVertexAttribPointer(attribLocation, attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		
		glVertexAttribPointer(attribLocation+1, attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glUseProgram(programID);
        
		glDrawArrays(GL_TRIANGLES, 0, buffer_size); // Starting from vertex 0; 3 vertices total -> 1 triangle
        return 1;
    }
}