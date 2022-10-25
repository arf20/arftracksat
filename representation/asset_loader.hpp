#pragma once

#include "../common/types-defs.hpp"

#include <vector>

#include <glad/glad.h>

struct Point {
    float x;
    float y;
    float z;
    float u;
    float v;
};

struct TexturedSphere {
    Point *points;
    size_t npoints;
    GLuint texture;
};

std::vector<shape> loadMap(const std::string& mapfilepath);
TexturedSphere loadEarthTextureSphere(const std::string& path);
