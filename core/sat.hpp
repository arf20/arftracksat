#pragma once

#include "../common/types-defs.hpp"
#include <vector>
#include <string>
#include <chrono>

struct compute_stats {
    time_t timeNow;
    tm utctime, loctime;
    float computeTime;
};

extern std::vector<sat> sats;
void loadSats(std::string tlefile);
compute_stats computeSats(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, size_t selsatidx);
