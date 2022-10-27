#pragma once

#include "../common/types-defs.hpp"
#include "colors.hpp"
#include "asset_loader.hpp"

#include <glm/glm.hpp>

#include <string>

#define TEXT_HEIGHT     15

std::string toString(float n);
void DrawString(xyz_t pos, std::string str, xyz_t c = C_WHITE);
float mercatorWidth(float scale);
float mercatorHeight(float scale);
glm::vec3 geoToMercatorCentered(xyz_t geo, float scale, float offx, float offy);
