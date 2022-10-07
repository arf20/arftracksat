#pragma once

#include "../common/types-defs.hpp"
#include "colors.hpp"
#include <string>

#define TEXT_HEIGHT     15

std::string toString(float n);
void DrawString(xyz_t pos, std::string str, xyz_t c = C_WHITE);
