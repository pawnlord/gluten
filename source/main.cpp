// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
// #define GLEW_STATIC

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include GLFW
#include <GLFW/glfw3.h>
using namespace glm;

#include "../include/GraphicsManager.h"
#include "../include/InputManager.h"
#include "../include/ResourceManager.h"
#include "../include/Shader.h"
#include "ObjectGenerator.h"

#include "config.h"

using namespace gluten;

int main() {
    GraphicsContext ctx{1024, 768, "Tutorial 01", 45.0};

    ShaderPipeline uvPipeline{SHADERS_FOLDER "uvvertexshader.glsl", SHADERS_FOLDER "uvfragmentshader.glsl", 0};

    ShaderPipeline colorPipeline{SHADERS_FOLDER "colorvertexshader.glsl", SHADERS_FOLDER "colorfragmentshader.glsl", 0};

    uvPipeline.addUniformVariable(Float1, "bs")
        ->addUniformVariable(Mat4, "MVP")
        ->addInShaderVariable(0, 3)
        ->addInShaderVariable(1, 2);

    colorPipeline.addUniformVariable(Float1, "bs")
        ->addUniformVariable(Mat4, "MVP")
        ->addInShaderVariable(0, 3)
        ->addInShaderVariable(1, 3);

    UVObject skewedCube{RES_FOLDER "test_obj.obj", std::shared_ptr<ShaderPipeline>(&uvPipeline), RES_FOLDER "textureExample.bmp"};

    skewedCube.internals.addBrightnessScalar("bs")
            ->addMVP("MVP")
            ->addVertexBuffer()
            ->addInShaderVariable(&skewedCube.uvbuffer);

    std::shared_ptr<NonUVObject> tris = ObjectGenerator().addPipeline(std::shared_ptr<ShaderPipeline>(&colorPipeline))
                     ->addTri(
                        glm::vec3(0.0, 0.0, 0.0),
                        glm::vec3(1.0, 0.0, 0.0),
                        glm::vec3(0.0, 0.0, 1.0),
                        glm::vec3(1.0, 0.0, 0.0)
                     )
                     ->addTri(
                        glm::vec3(0.0, 0.0, 0.0),
                        glm::vec3(1.0, 0.0, 0.0),
                        glm::vec3(0.0, 1.0, 0.0),
                        glm::vec3(1.0, 1.0, 0.0)
                     )
                     ->generate(); 

    GLfloat t = 0;


    InputInfo inputInfo;
    Camera camera;
    
    skewedCube.internals.addUpdate([&](auto model) {
        model = mat4(1);
        model = glm::translate(model, vec3(-2.0, 0, 0));
        return model;
    });

    registerInputInfo(inputInfo);

    inputInfo.setBrightnessScalar(&skewedCube.internals.brightnessScalar);

    glfwSetScrollCallback(ctx.window, scrollCallback);

    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        computeMatricesFromInputs(ctx.window, camera, inputInfo);

        ctx.view = camera.getViewMatrix();
        ctx.projection = camera.getProjectionMatrix();

        tris->display(ctx);
        
        skewedCube.display(ctx);
        glDisableVertexAttribArray(0);
        

        glfwSwapBuffers(ctx.window);
        glfwPollEvents();
        t += (GLfloat)0.0005;

    }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ctx.window) == 0);
}