#pragma once

#include "../common/types-defs.hpp"

#include "tiny_obj_loader.h"

#include <vector>

struct obj {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
};

std::vector<shape> loadMap(const std::string& mapfilepath);
obj loadEarth(std::string& objpath);
