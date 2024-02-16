// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
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
#include "TerrainGenerator.h"

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

    UVObject cube{RES_FOLDER "test_obj.obj", std::shared_ptr<ShaderPipeline>(&uvPipeline), RES_FOLDER "textureExample.bmp"};
    cube.internals.brightnessScalar = 3;
    cube.internals.addBrightnessScalar("bs")
            ->addMVP("MVP")
            ->addVertexBuffer()
            ->addInShaderVariable(&cube.uvbuffer);
    TerrainConfig cfg;
    
    cfg.noiseScale = 0.01f;
    cfg.xMin = 0.0;
    cfg.xMax = 110.0;
    cfg.yMin = 0.0;
    cfg.yMax = 110.0;
    cfg.zMin = 0.0;
    cfg.zMax = 20.0;
    cfg.size = 0.1f;

    cfg.red = [&](float height) {
        return (1.0/(glm::abs(height) + 1) - 0.5) * 2;
    };

    cfg.green = [&](float height) {
        if(height < 0.80) {
            return (float)(exp(-(height*8.0 - 4))/pow(1 + exp(-(height*8.0 - 4)), 2)) * 4;
        } else {
            return (float)(1.0/(glm::abs(height) + 1) - 0.5) * 2;
        }
    };
    cfg.blue = [&](float height) {
        if(height < 0.64) {
            return 0.1f;
        } else {
            return (float)(1.0/(glm::abs(height) + 1) - 0.5) * 2;
        }
    };

    Terrain terrain{
        cfg, std::shared_ptr<ShaderPipeline>(&colorPipeline)
    };

    GLfloat t = 0;


    InputInfo inputInfo;
    Camera camera;
    
    cube.internals.addUpdate([&](auto model) {
        model = mat4(1);
        model = glm::translate(model, vec3(-4.0, 0, 0));
        return model;
    });
    

    registerInputInfo(inputInfo);

    // inputInfo.setBrightnessScalar(&cube.internals.brightnessScalar);

    glfwSetScrollCallback(ctx.window, scrollCallback);

    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    auto last_time = std::chrono::system_clock::now();
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        computeMatricesFromInputs(ctx.window, camera, inputInfo);
        ctx.view = camera.getViewMatrix();
        ctx.projection = camera.getProjectionMatrix();

        terrain.display(ctx);
        cube.display(ctx);

        glDisableVertexAttribArray(0);
        
        auto time = std::chrono::system_clock::now();

        glFinish();
        std::chrono::duration<double> diff = (time - last_time);
        // std::cout << 1.0/diff.count() << std::endl;

        last_time = time;
        glfwSwapBuffers(ctx.window);
        glfwPollEvents();
        t += (GLfloat)0.0005;

    } while (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ctx.window) == 0);
}