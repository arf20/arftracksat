#pragma once

#include "glutil.hpp"

void textRendererInit(const char *vspath, const char *fspath, const char *texpath, int lcw, int lch, const int *lsw, const int *lsh);
void addText(const char *str, int x, int y, glm::vec3 c);
void renderText();
