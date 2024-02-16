#include "TerrainGenerator.h"


#include "ObjectGenerator.h"
#include <stdlib.h>
#include <time.h>
#include <PerlinNoise.hpp>
namespace gluten {

siv::PerlinNoise perlin{0};
siv::PerlinNoise secondOrderPerlin{0};
float noise(float x, float y, TerrainConfig cfg) {
    double secondOrder = secondOrderPerlin.octave2D_01(x*(cfg.noiseScale), y*(cfg.noiseScale), 4);
    return (float)perlin.octave2D_01(x*cfg.noiseScale, y*cfg.noiseScale, 4) * secondOrder * secondOrder;
}

Terrain::Terrain(TerrainConfig cfg, std::shared_ptr<ShaderPipeline> pipeline) {
    perlin = siv::PerlinNoise{(unsigned int)time(NULL)};
    secondOrderPerlin = siv::PerlinNoise{(unsigned int)time(NULL)*2};
    float max = 0.0;
    float min = 1.0;
    for (float x = cfg.xMin; x < cfg.xMax; x += cfg.size) {
        points.push_back(std::vector<vec3>{});
        for (float y = cfg.yMin; y < cfg.yMax; y += cfg.size) {\
            float height = noise(x, y, cfg);
            if (height > max) {
                max = height;
            }
            if (height < min) {
                min = height;
            }
            points.back().push_back(vec3(x, height, y));
        }
    }
    
    float scale = (cfg.zMax-cfg.zMin)/(max-min);
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points[i].size(); j++) {
            vec3 p = points[i][j];
            points[i][j] = vec3(p.x, scale * p.y, p.z);
        }
    }


    this->pipeline = pipeline;

    this->xMin = cfg.xMin;
    this->xMax = cfg.xMax;
    this->yMin = cfg.yMin;
    this->yMax = cfg.yMax;
    this->zMin = cfg.zMin;
    this->zMax = cfg.zMax;

    this->red = cfg.red;
    this->green = cfg.green;
    this->blue = cfg.blue;
    
    generateObject();
}

void Terrain::generateObject() {
    ObjectGenerator generator;
    generator.addPipeline(this->pipeline);
    double scale = (zMax - zMin);
    for (int i = 0; i < points.size()-1; i++) {
        for (int j = 0; j < points[i].size()-1; j++) {
            float avg_height = (points[i][j].y 
                        + points[i+1][j].y 
                        + points[i][j+1].y 
                        + points[i+1][j+1].y)/(4 * scale);
            generator.addQuad(
                points[i][j],
                points[i+1][j],
                points[i][j+1],
                points[i+1][j+1],
                vec3(red(avg_height), green(avg_height), blue(avg_height))
            );
        }
    }


    graphicsObject = generator.generate();

    graphicsObject->internals.brightnessScalar = 10;
}
void Terrain::display(GraphicsContext ctx) {
    graphicsObject->display(ctx);
}


float defaultTerrainColor(float height) {
  return height;
}

}