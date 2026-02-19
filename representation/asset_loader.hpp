#pragma once

#include "../common/types-defs.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>

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
