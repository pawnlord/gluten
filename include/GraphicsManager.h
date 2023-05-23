#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#define GLEW_STATIC
#include <vector>
#include <string>
#include <functional>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "../include/GraphicsContext.h"
using namespace glm;

namespace gm {

    GLuint setupGraphicsManager(GLFWwindow* window, glm::mat4& Projection, float projectionAngle);
    GLuint drawColoredArray(int attribLocation, int attribSize, 
                        GltnGraphicsContext, glm::mat4 Model,
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size
                    );
    GLuint drawTexturedArray(int attribLocation, int attribSize, 
                        GltnGraphicsContext, glm::mat4 Model,
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size
                    );
    GLuint genBuffer2(GLuint* buffer, GLuint buffer_size, std::vector<vec2> v);
    GLuint genBuffer3(GLuint* buffer, GLuint buffer_size, std::vector<vec3> v);
    GLuint genBuffer(GLuint* buffer, GLuint buffer_size, GLfloat* buffer_data);
    GLuint loadTexture(GLuint textureID);

    class GltnFileObject {
        virtual void load(std::string path) = 0;
        virtual void display(GltnGraphicsContext) = 0;
            
    };

    class GltnUVObject : public GltnFileObject {
        public:
            std::vector<vec3> normals;
            std::vector<vec3> vertices;
            std::vector<vec2> uvs;
            glm::mat4 mvp;

            std::shared_ptr<shader::GltnShaderPipeline> pipeline;
            float brightnessScalar;

            GLuint vertexbuffer;
	        GLuint uvbuffer;
            GLuint textureID = 0;

            int t = 0;
		    glm::mat4 model = glm::mat4(1.0f);
            GltnUVObject(std::string path, std::shared_ptr<shader::GltnShaderPipeline> pipeline) 
                : pipeline{pipeline}, brightnessScalar{1.0f} {
                load(path);
            }
            void load(std::string path) override;
            void display(GltnGraphicsContext) override;
            void updateModel(std::function<void(glm::mat4& model)> fp);
            void usingTexture(std::string);
    };

}
#endif