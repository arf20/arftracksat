#include "main.h"

#include "sgdp4/sgdp4.h"

#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

#define TORAD	PI/180.0
#define TODEG	180.0/PI

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

void computeSats(time_t t) {
	for (sat& sat : sats) {
		double mse;
		// init
		sgdp4_prediction_t pred;
		xyz_t stageo = geodegtorad(sta.geo);
		sgdp4_prediction_init(&pred, &sat.orbit, &stageo);

		// propagate
		timeval tv;
		tv.tv_sec = t; tv.tv_usec = 0;
		sgdp4_prediction_update(&pred, &tv);

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
		// Difference in coords
		double deltaLat = sat.geo.lat - sta.geo.lat;
		double deltaLon = sat.geo.lon - sta.geo.lon;

		// Formulae for azimuth
		sat.aer.azimuth = TODEG * atan2(
			sin(TORAD * deltaLon) * cos(TORAD * sat.geo.lat),
			(cos(TORAD * sta.geo.lat) * sin(TORAD * sat.geo.lat) - (sin(TORAD * sta.geo.lat) * cos(TORAD * sat.geo.lat) * cos(TORAD * deltaLon)))
		);

		// Correct negative azimuths
		while (sat.aer.azimuth < 0.0)
			sat.aer.azimuth += 360.0;


		// == EL ==
		// Haversine formulae
		double h = pow(sin((TORAD * deltaLat) / 2.0), 2)
			+ (cos(TORAD * sta.geo.lat) * cos(TORAD * sat.geo.lat) * pow(sin((TORAD * deltaLon) / 2.0), 2));	// haversine of theta
		double theta = 2.0 * atan2(sqrt(h), sqrt(1.0 - h));															// arc angle between 2 coords (rad)
		double gcd = EARTHR * theta;																				// arc length

		// Triangle:  earth center (C), observer (O) and satellite (S)
		double oRLen = EARTHR + sta.geo.height;																		// CO length
		double sRLen = EARTHR + sat.geo.height;																		// CS length
			
		double osLen = sqrt(pow(oRLen, 2) + pow(sRLen, 2) - (2 * oRLen * sRLen * cos(theta)));						// OS length (missing side)

		// Find CO OS angle
		double phi = asin((sRLen * sin(theta)) / osLen);

		// Elevation is that angle minus 90�
		sat.aer.elevation = (TODEG * phi) - 90;



		// Radius (range) to sat
		sat.aer.distance = osLen;
		// Great circle distance
		//sat.aer.gcd = gcd;


		sat.rVel = xyzdot(sat.vel - sta.vel, xyzunit(sat.pos - sta.pos));
		sat.dopShift = (sat.rVel / LIGHTC) * 137500000.0;
	}
}