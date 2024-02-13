#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
// #define GLEW_STATIC
#include <vector>
#include <string>
#include <functional>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "../include/GraphicsContext.h"
using namespace glm;

namespace gluten {

    GLuint setupGraphicsManager(GLFWwindow* window, glm::mat4& Projection, float projectionAngle);
    GLuint drawColoredArray(int attribLocation, int attribSize, 
                        GraphicsContext, glm::mat4 Model,
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size
                    );
    GLuint drawTexturedArray(int attribLocation, int attribSize, 
                        GraphicsContext, glm::mat4 Model,
                        GLuint programID, GLuint vertexbuffer, GLuint colorbuffer,
                        int buffer_size
                    );
    GLuint genBuffer2(GLuint* buffer, GLuint buffer_size, std::vector<vec2> v);
    GLuint genBuffer3(GLuint* buffer, GLuint buffer_size, std::vector<vec3> v);
    GLuint genBuffer(GLuint* buffer, GLuint buffer_size, GLfloat* buffer_data);
    GLuint loadTexture(GLuint textureID);

    class GltnObject {
        virtual void load(std::string path) = 0;
        virtual void display(GraphicsContext) = 0;
    };


    // Object representing internal data of a displayed object, including the render pipeline
    // and update information
    // This is the data common to all objects
    // By being a separate data type, it allows us to pass it in to any other object we create
    struct ObjectInternal {
        std::shared_ptr<gluten::ShaderPipeline> pipeline;
        glm::mat4 mvp;
        std::vector<vec3> normals;
        std::vector<vec3> vertices;
        std::unordered_map<std::string, void *> uniformValues;
        std::vector<GLuint*> buffers;
        std::vector<std::function<void(glm::mat4& model)>> updates;
        GLuint vertexbuffer;
        float brightnessScalar = 1.0;
        glm::mat4 model = glm::mat4(1.0f);
        
        ObjectInternal(std::shared_ptr<gluten::ShaderPipeline> pipeline): pipeline{pipeline} {} 
        
        // Add to the update process
        ObjectInternal *addUpdate(std::function<void(glm::mat4& model)> fp);
        
        // Update and display model
        void updateModel(std::function<void(glm::mat4& model)> fp);
        void display(GraphicsContext ctx);
        

        // Add to the shader pipeline
        ObjectInternal *addBrightnessScalar(std::string name);
        ObjectInternal *addMVP(std::string name);
        ObjectInternal *addVertexBuffer();
        ObjectInternal *addUniformVariable(std::string name, void *data);
        ObjectInternal *addInShaderVariable(GLuint *buffer);
    };

    class UVObject : public GltnObject {
        public:
            std::vector<vec2> uvs;

            ObjectInternal internals;
	        GLuint uvbuffer;
            GLuint textureID = 0;

            UVObject(std::string path, std::shared_ptr<gluten::ShaderPipeline> pipeline, std::string texturePath = "") 
                   : internals{pipeline}{
                load(path);
                usingTexture(texturePath);
            }

            UVObject(ObjectInternal internals, std::string texturePath = "") 
                    : internals{internals} {
                usingTexture(texturePath);
            }

            void load(std::string path) override;
            void display(GraphicsContext) override;
            void usingTexture(std::string);
            void updateModel(std::function<void(glm::mat4& model)> fp);

    };
    class NonUVObject : public GltnObject {
        public:
            std::vector<vec3> colors;
	        GLuint colorbuffer;
            ObjectInternal internals;
            void load(std::string path) override;
            void display(GraphicsContext) override;
            
            void updateModel(std::function<void(glm::mat4& model)> fp){
                internals.updateModel(fp);
            }

            NonUVObject(std::string path, std::shared_ptr<gluten::ShaderPipeline> pipeline) 
                    : internals{pipeline} {
                load(path);
            }
            
            
            NonUVObject(ObjectInternal internals, std::vector<vec3> colors) 
                    : internals{internals}, colors{colors} {
                genBuffer3(&internals.vertexbuffer, internals.vertices.size()*6*3, internals.vertices);
                genBuffer3(&colorbuffer, colors.size()*6*3, colors);
            }
    };


}
#endif