#pragma once

//#include "main.hpp"
#include "types-defs.hpp"
#include <vector>
#include <string>

extern std::vector<sat> sats;
void loadSats(std::string tlefile);
void computeLoop(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, size_t selsatidx);
void computeSats(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, size_t selsatidx);