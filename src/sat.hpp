#pragma once

#include "main.hpp"
#include <vector>
#include <string>

extern std::vector<sat> sats;
void loadSats(std::string tlefile);
void computeSats(time_t t);