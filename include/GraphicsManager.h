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

    class GltnObject {
        virtual void load(std::string path) = 0;
        virtual void display(GltnGraphicsContext) = 0;
    };

    // Object representing internal data, unrelated to loading and drawing
    // This may be updated as we go
    struct GltnInternal {
        std::shared_ptr<shader::GltnShaderPipeline> pipeline;
        glm::mat4 mvp;
        std::vector<vec3> normals;
        std::vector<vec3> vertices;
        std::unordered_map<std::string, void *> uniformValues;
        std::vector<GLuint*> buffers;
        GLuint vertexbuffer;
        float brightnessScalar = 1.0;
        glm::mat4 model = glm::mat4(1.0f);
        GltnInternal(std::shared_ptr<shader::GltnShaderPipeline> pipeline): pipeline{pipeline} {} 
        void updateModel(std::function<void(glm::mat4& model)> fp);
        void display(GltnGraphicsContext ctx);
        GltnInternal *addBrightnessScalar(std::string name);
        GltnInternal *addMVP(std::string name);
        GltnInternal *addVertexBuffer();
        GltnInternal *addUniformVariable(std::string name, void *data);
        GltnInternal *addInShaderVariable(GLuint *buffer);
    };

    class GltnUVObject : public GltnObject {
        public:
            std::vector<vec2> uvs;

            GltnInternal internals;
	        GLuint uvbuffer;
            GLuint textureID = 0;

            GltnUVObject(std::string path, std::shared_ptr<shader::GltnShaderPipeline> pipeline) 
                : internals{pipeline}{
                load(path);
            }
            void load(std::string path) override;
            void display(GltnGraphicsContext) override;
            void usingTexture(std::string);
            void updateModel(std::function<void(glm::mat4& model)> fp);

    };
    class GltnNonUVObject : public GltnObject {
        public:
            std::vector<vec3> colors;
	        GLuint colorbuffer;
            GltnInternal internals;
            void load(std::string path) override;
            void display(GltnGraphicsContext) override;
            
            void updateModel(std::function<void(glm::mat4& model)> fp){
                internals.updateModel(fp);
            }

            GltnNonUVObject(std::string path, std::shared_ptr<shader::GltnShaderPipeline> pipeline) 
                : internals{pipeline}{
                load(path);
            }
            
    };
}
#endif