#pragma once

#include "../common/types-defs.hpp"
#include <vector>
#include <chrono>
#include <ctime>
#include <string>

void renderRepresentation(int w, int h);
void startRepresentation(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile, std::string objfile);
