#pragma once

#include "types-defs.hpp"
#include <vector>
#include <chrono>
#include <ctime>
#include <string>


#define TORAD	PI/180.0
#define TODEG	180.0/PI

extern tm utctime, loctime, aosloctime, aosutctime, losloctime, losutctime;
extern std::chrono::nanoseconds g_computeTime;
extern int g_selsatidx;     // index of shownSats
void startGraphics(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile, std::string objfile);