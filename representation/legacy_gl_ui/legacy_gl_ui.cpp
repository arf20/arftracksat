
#include "../../common/types-defs.hpp"
#include "../colors.hpp"

#include "../graphic_util.hpp"

#include <glad/glad.h>
#include "../textrenderer.hpp"

#include <string>

#define SATLIST_SIZE    10

void legacy_gl_ui(float width, float height, float deltaTime, float computeTime, bool mode, time_t time, const station& sta, const std::vector<std::vector<sat>::iterator>& shownSats, int selsatidx) {
    // Draw info
    float fps = 1 / deltaTime;
    std::string modestr = mode ? "PERSPECTIVE" : "MERCATOR";
    DrawString({20, 20}, "FPS: " + toString(fps) + "  COMP TIME: " + toString(computeTime * 1000.0f) + "ms  MODE: " + modestr);

	// Station column
	xyz_t curpos{ 200.0f + (width / 2.0f), 50.0 };
	float subcolspacing = 100;

	DrawString(curpos, "STATION " + sta.name); curpos.y += 2.0f * TEXT_HEIGHT;

	DrawString(curpos, "LAT"); curpos.y += TEXT_HEIGHT; 
	DrawString(curpos, toString(sta.geo.lat)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "LON"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.geo.lon)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "HGT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.geo.height)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.pos.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.pos.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.pos.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;    

    tm utctime, loctime, aosloctime, aosutctime, losloctime, losutctime;
    gmtime_r(&time, &utctime);
    localtime_r(&time, &loctime);

    DrawString(curpos, "TIME    UTC    " + std::to_string(utctime.tm_hour) + ":" + std::to_string(utctime.tm_min) + ":" + std::to_string(utctime.tm_sec)); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "        LOCAL  " + std::to_string(loctime.tm_hour) + ":" + std::to_string(loctime.tm_min) + ":" + std::to_string(loctime.tm_sec)); curpos.y += 4.0f * TEXT_HEIGHT;

    // Sat column

    DrawString(curpos, "SATELLITE (" + std::to_string(shownSats.size()) + ")"); curpos.y += 2.0f * TEXT_HEIGHT;

    for (int i = selsatidx; i < shownSats.size(); i++) {
        if (i + 1 > selsatidx + SATLIST_SIZE) break;
        auto sat = shownSats[i];
        xyz_t c = C_WHITE;
		if (i == selsatidx) { c = C_YELLOW; }

		DrawString(curpos, std::to_string(i + 1) + ". " + sat->name, c);

		curpos.y += TEXT_HEIGHT;
    }

    auto selsat = shownSats[selsatidx];

    gmtime_r(&selsat->aos, &aosutctime);
    localtime_r(&selsat->aos, &aosloctime);
    gmtime_r(&selsat->los, &losutctime);
    localtime_r(&selsat->los, &losloctime);

	curpos.y += 2.0f * TEXT_HEIGHT;

    // AOS
    curpos.y += TEXT_HEIGHT;
    DrawString(curpos, "UTC"); curpos.y += TEXT_HEIGHT;
    DrawString(curpos, "LOCAL"); curpos.x += subcolspacing; curpos.y -= 2.0f * TEXT_HEIGHT;
    DrawString(curpos, "AOS"); curpos.x += subcolspacing;
    DrawString(curpos, "LOS"); curpos.x -= subcolspacing; curpos.y += TEXT_HEIGHT;
    DrawString(curpos, std::to_string(aosutctime.tm_hour) + ":" + std::to_string(aosutctime.tm_min) + ":" + std::to_string(aosutctime.tm_sec)); curpos.x += subcolspacing;
	DrawString(curpos, std::to_string(losutctime.tm_hour) + ":" + std::to_string(losutctime.tm_min) + ":" + std::to_string(losutctime.tm_sec)); curpos.x -= subcolspacing; curpos.y += TEXT_HEIGHT;
    DrawString(curpos, std::to_string(aosloctime.tm_hour) + ":" + std::to_string(aosloctime.tm_min) + ":" + std::to_string(aosloctime.tm_sec)); curpos.x += subcolspacing;
	DrawString(curpos, std::to_string(losloctime.tm_hour) + ":" + std::to_string(losloctime.tm_min) + ":" + std::to_string(losloctime.tm_sec)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // AZEL
    DrawString(curpos, "AZ"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->aer.azimuth)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "EL"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->aer.elevation)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "DIS"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->aer.distance)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // GEO
	DrawString(curpos, "LAT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->geo.lat)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "LON"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->geo.lon)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "HGT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->geo.height)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // ECEF
	DrawString(curpos, "POS"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->pos.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->pos.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->pos.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // VEL ECEF
	DrawString(curpos, "VEL"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->vel.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->vel.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->vel.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // DOPPLER
	/*DrawString(curpos, "FREQ"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, std::to_string(selsat->freq / 1000000.0)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "DOPPLER"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, std::to_string(selsat->doppler)); curpos.x -= subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;
    
    
	DrawString(curpos, "COMPUTE TIME"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, std::to_string(computeTime)); curpos.y += TEXT_HEIGHT;*/
    //DrawString(curpos, std::to_string(rotatex)); curpos.y += TEXT_HEIGHT;
    //DrawString(curpos, std::to_string(rotatez)); curpos.y += TEXT_HEIGHT;
}
