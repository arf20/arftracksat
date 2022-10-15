#include "asset_loader.hpp"

#include "../common/types-defs.hpp"

#include <nlohmann/json.hpp>
using namespace nlohmann;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/freeglut.h>

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

std::vector<shape> loadMap(const std::string& mapfilepath) {
	std::vector<shape> continents;

	std::ifstream mapfile(mapfilepath);
	json geojson;
	try {
		mapfile >> geojson;
		json map = geojson["features"][0]["geometry"]["coordinates"];
	}
	catch (json::exception& e) {
		std::cout << "Malformed (Geo)JSON map: " + std::string(e.what()) << std::endl;
		return continents;
	}

	std::vector<std::vector<std::vector<std::vector<double>>>> tempmap; // last vector could be a pair...

	try {
		tempmap = geojson["features"][0]["geometry"]["coordinates"].get<std::vector<std::vector<std::vector<std::vector<double>>>>>();
	}
	catch (json::exception& e) {
		std::cout << "Malformed (Geo)JSON map: " + std::string(e.what()) << std::endl;
		return continents;
	}

	for (auto &a : tempmap) {
		for (auto &b : a) {
			shape shape;
			for (auto &c : b) {
				shape.points.push_back(xyz_t{ c[0], c[1], 0.0f });
			}
			continents.push_back(shape);
		}
	}

	int pointcount = 0;
	for (shape &shape : continents)
        pointcount += shape.points.size();

	std::cout << "Map loaded [" << pointcount << " points]" << std::endl;
	return continents;
}

const float pi = std::atan(1.0f) * 4.0f;

// Generate point from the u, v coordinates in (0..1, 0..1)
Point sphere_point(float u, float v) {
    float r = std::sin(pi * v);
    return Point{
        r * std::cos(2.0f * pi * u),
        r * std::sin(2.0f * pi * u),
        std::cos(pi * v),
        u,
        v
    };
}

// Create array of points with quads that make a unit sphere.
Point *sphere(int hSize, int vSize) {
    Point *pt = new Point[hSize * vSize * 6];
	int idx = 0;
    for (int i = 0; i < hSize; i++) {
        for (int j = 0; j < vSize; j++) {
            float u0 = (float)i / (float)hSize;
            float u1 = (float)(i + 1) / (float)hSize;
            float v0 = (float)j / (float)vSize;
            float v1 = (float)(j + 1) / float(vSize);
            // Create quad as two triangles.
            pt[idx] = sphere_point(u0, v0); idx++;
            pt[idx] = sphere_point(u1, v0); idx++;
            pt[idx] = sphere_point(u0, v1); idx++;
            pt[idx] = sphere_point(u0, v1); idx++;
            pt[idx] = sphere_point(u1, v0); idx++;
            pt[idx] = sphere_point(u1, v1); idx++;
        }
    }
	return pt;
}

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        abort();
    }
}

#define _DEBUG
#ifdef _DEBUG
    #define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
    #define GL_CHECK(stmt) stmt
#endif

TexturedSphere loadEarthTextureSphere(const std::string& path) {
	TexturedSphere ts;

	glCullFace(GL_FRONT);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data) { std::cout << "Error loading texture data " << path << ": " << data << std::endl; exit(1); }
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
	stbi_image_free(data);

	glActiveTexture(texture);

	ts.texture = texture;
	ts.npoints = 32 * 32 * 6;
	ts.points = sphere(32, 32);

	std::cout << "Earth loaded [" << ts.npoints << " points]" << std::endl;

	return ts;
}
