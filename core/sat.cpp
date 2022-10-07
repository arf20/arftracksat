#include "sat.hpp"

//#include "graphics.hpp"
#include "../common/util.hpp"
#include "../common/sgdp4/sgdp4.h"

#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

std::vector<sat> sats;

inline xyz_t georadtodeg(xyz_t geo) {
	return xyz_t{TODEG * geo.x, TODEG * geo.y, geo.z};
}

inline xyz_t geodegtorad(xyz_t geo) {
	return xyz_t{TORAD * geo.x, TORAD * geo.y, geo.z};
}

void loadSats(std::string tlefile) {
	std::ifstream file(tlefile, std::ios::binary | std::ios::ate);
	if (file.bad()) {
		std::cout << "Error opening TLE file" << std::endl;
	}

	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	char *buff = (char*)malloc(size);
	file.read(buff, size);

	orbit_t orbit;
	sat sat;
	while (true) {
		size_t consumed = orbit_init_from_data(&orbit, buff, size);
		if (consumed == 0 || consumed > size) break;

		sat.name = std::string(orbit.name);
		sat.norad = orbit.satno;
		sat.orbit = orbit;

		buff += consumed;

		sats.push_back(sat);
	}

	std::cout << "Satellites loaded" << std::endl;
}

compute_stats computeSats(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, size_t selsatidx) {
	compute_stats stats;
	timeval tv_now = getTime();
	gmtime_r(&tv_now.tv_sec, &stats.utctime);
	localtime_r(&tv_now.tv_sec, &stats.loctime);
	stats.timeNow = tv_now.tv_sec;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < shownSats.size(); i++) {
		timeval t = tv_now;
		
		sat& sat = *shownSats[i];
		// init
		sgdp4_prediction_t pred;
		xyz_t stageo = geodegtorad(sta.geo);
		sgdp4_prediction_init(&pred, &sat.orbit, &stageo);

		// propagate
		sgdp4_prediction_update(&pred, &t);

		// get stuff
		sgdp4_prediction_get_ecef(&pred, &sat.pos);
		sgdp4_prediction_get_vel_ecef(&pred, &sat.vel);
		sgdp4_prediction_get_azel(&pred, &sat.aer);
		xyz_ecef_to_geodetic(&sat.pos, &sat.geo);
		sat.geo = georadtodeg(sat.geo);

		while (sat.geo.lon < -180.0)
			sat.geo.lon = sat.geo.lon + 360.0;

		while (sat.geo.lon > (180.0))
			sat.geo.lon = sat.geo.lon - 360.0;

		// == AZ ==
		xyz_t u_lat = uLat(sta.geo); xyz_t u_lon = uLon(sta.geo); xyz_t u_vert = uVert(sta.geo);
		xyz_t osDistanceV = sat.pos - sta.pos;

		double azO = xyzdot(u_lon, osDistanceV);
		double azC = xyzdot(u_lat, osDistanceV);

		double theta = TODEG * atan2(azO, azC);

		// Correct negative azimuths
		while (theta < 0.0)
			theta += 360.0;

		sat.aer.azimuth = theta;

		// == EL ==
		double phi = acos(xyzdot(u_vert, osDistanceV) / xyzmod(osDistanceV));

		// Elevation is 
		sat.aer.elevation = 90.0 - (TODEG * phi);

		// Radius (range) to sat
		sat.aer.distance = xyzmod(osDistanceV);

		float rVel = xyzdot(sat.vel - sta.vel, xyzunit(sat.pos - sta.pos));
		sat.doppler = (rVel / LIGHTC) * 137500000.0;

		// Find AOS & LOS within a day
		timeval tv_aos;
		timeval tv_los;
		sgdp4_prediction_find_aos(&pred, &t, 24 * 60 * 60, &tv_aos);
		sgdp4_prediction_find_los(&pred, &t, 24 * 60 * 60, &tv_los);
		sat.aos = tv_aos.tv_sec;
		sat.los = tv_los.tv_sec;

		// Find AOS radius
		sat.aosRadiusAngle = TODEG * acos(EARTHR / (EARTHR + sat.geo.height));

		if (i == selsatidx) {
			// Compute orbit
			sat.geoOrbit.points.clear();
			float orbitalPeriod = (1.0f / sat.orbit.rev) * 24.0f * 60.0f * 60.0f;
			time_t orbitStart = t.tv_sec - orbitalPeriod;
			time_t orbitEnd = t.tv_sec + orbitalPeriod;
			xyz_t t_ecef;
			xyz_t t_geo;
			for (t.tv_sec = orbitStart; t.tv_sec <= orbitEnd; t.tv_sec += 60) {
				sgdp4_prediction_update(&pred, &t);	// propagate
				sgdp4_prediction_get_ecef(&pred, &t_ecef);
				xyz_ecef_to_geodetic(&t_ecef, &t_geo);
				t_geo = georadtodeg(t_geo);
				sat.geoOrbit.points.push_back(t_geo);
			}
		}

		sgdp4_prediction_finalize(&pred);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	stats.computeTime = (stop - start).count() / 1000000000.0f;

	return stats;
}