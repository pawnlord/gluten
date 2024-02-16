#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "GraphicsManager.h"

namespace gluten {

float defaultTerrainColor(float height);

struct TerrainConfig {
    float noiseScale = 0.0;
    float xMin = 0.0;
    float xMax = 0.0;
    float yMin = 0.0;
    float yMax = 0.0;
    float zMin = 0.0;
    float zMax = 0.0;
    float size = 1.0;

    std::function<float(float)> red = defaultTerrainColor;
    std::function<float(float)> green = defaultTerrainColor;
    std::function<float(float)> blue = defaultTerrainColor;
};

class Terrain {
  public:
    Terrain(TerrainConfig cfg, std::shared_ptr<ShaderPipeline> pipeline);

    void display(GraphicsContext ctx);

    // TODO: noise algorithm? editing points? regenerating?
    // scaling?

  private:
    std::shared_ptr<NonUVObject> graphicsObject;

    float xMin;
    float xMax;

    float yMin;
    float yMax;

    float zMin;
    float zMax;

    float size;

    std::function<float(float)> red;
    std::function<float(float)> green;
    std::function<float(float)> blue;

    std::shared_ptr<ShaderPipeline> pipeline;

    std::vector<std::vector<glm::vec3>> points;
    void generateObject();
};


} // namespace gluten

#endif // TERRAIN_GENERATOR_H