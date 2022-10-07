#define TINYOBJLOADER_IMPLEMENTATION
#include "asset_loader.hpp"

#include "../common/types-defs.hpp"

#include <nlohmann/json.hpp>
using namespace nlohmann;

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

std::vector<shape> loadMap(const std::string& mapfilepath) {
	std::cout << "Loading GeoJSON map..." << std::endl;

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

obj loadEarth(std::string& objpath) {
	obj o;

    std::filesystem::path path(objpath);
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = path.parent_path(); // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(objpath, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        return o;
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    o.attrib = reader.GetAttrib();
    o.shapes = reader.GetShapes();
    o.materials = reader.GetMaterials();
	return o;
}
