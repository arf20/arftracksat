#pragma once

#include "../common/types-defs.hpp"
#include "colors.hpp"
#include "asset_loader.hpp"

#include <string>

#define TEXT_HEIGHT     15

std::string toString(float n);
void DrawString(xyz_t pos, std::string str, xyz_t c = C_WHITE);
float mercatorWidth(float scale);
float mercatorHeight(float scale);
xyz_t geoToMercatorCentered(xyz_t geo, float scale, float offx, float offy);
