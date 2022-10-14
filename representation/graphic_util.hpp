#pragma once

#include "../common/types-defs.hpp"
#include "colors.hpp"
#include "asset_loader.hpp"

#include <string>

#include <GL/freeglut.h>
#include <GL/gl.h>

#define TEXT_HEIGHT     15

std::string toString(float n);
void DrawString(xyz_t pos, std::string str, xyz_t c = C_WHITE);
void DrawShape(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c = C_WHITE);
float mercatorWidth(float scale);
float mercatorHeight(float scale);
xyz_t geoToMercatorCentered(xyz_t geo, float scale, float offx, float offy);
void DrawGeoLines(std::vector<xyz_t>& lines, float scale, float offx, float offy, xyz_t c = C_WHITE);
void DrawGeoShape(const std::vector<xyz_t>& shape, float scale, float offx, float offy, xyz_t c = C_WHITE);
void DrawGeoLines3(const std::vector<xyz_t>& lines, float scale, xyz_t c = C_WHITE);
void DrawGeoShape3(const std::vector<xyz_t>& shape, float scale, xyz_t c = C_WHITE);
void DrawBillboardShape3(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c = C_WHITE);
void DrawTexturedSphere(TexturedSphere sphere, float radius, float roty);
