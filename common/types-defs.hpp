#pragma once

#include "sgdp4/sgdp4.h"

#include <vector>
#include <string>

struct shape {
	std::vector<xyz_t> points;
};

struct sat {
	std::string name;
	std::string norad;
	std::string intid;
	orbit_t orbit;
	xyz_t pos;
	xyz_t vel;
	xyz_t geo;
	xyz_t aer;
	double doppler;
	time_t aos;
	time_t los;
	shape geoOrbit;
	float aosRadiusAngle;
};

struct station {
	std::string name;
	xyz_t pos;
	xyz_t vel;
	xyz_t geo;
};

#define TORAD	PI/180.0
#define TODEG	180.0/PI

#define EARTHR	6371.0
#define EQR		6378.0
#define POR		6356.8

#define LIGHTC	299792458.0

// xyz_t C++ utils
inline xyz_t operator+(const xyz_t left, const xyz_t right) {
  return xyz_t{ left.x + right.x, left.y + right.y, left.z + right.z };
}

inline xyz_t operator-(const xyz_t left, const xyz_t right) {
  return xyz_t{ left.x - right.x, left.y - right.y, left.z - right.z };
}

inline xyz_t operator*(const xyz_t left, double right) {
  return xyz_t{ left.x * right, left.y * right, left.z * right };
}

inline xyz_t operator/(const xyz_t left, double right) {
  return xyz_t{ left.x / right, left.y / right, left.z / right };
}

inline double xyzmod(xyz_t v) {
  return std::hypot(v.x, v.y, v.z);
}

inline xyz_t xyzunit(xyz_t v) {
	return v / xyzmod(v);
}

inline double xyzdot(xyz_t left, xyz_t right) {
  return ((left.x * right.x) + (left.y * right.y) + (left.z * right.z));
}

inline xyz_t geoToECEF(xyz_t geo) {
    xyz_t t;
    geo.lat = geo.lat * TORAD;
    geo.lon = geo.lon * TORAD;
    xyz_geodetic_to_ecef(&geo, &t);
    return t;
}

inline xyz_t ECEFToGeo(xyz_t pos) {
    xyz_t t;
    xyz_ecef_to_geodetic(&pos, &t);
    t.lat = t.lat * TODEG;
    t.lon = t.lon * TODEG;
    return t;
}

#define EARTHECC2 .006694385000 /* Eccentricity of Earth^2 */

inline xyz_t uLat(xyz_t geo) {
    geo.lat = geo.lat * TORAD;
    geo.lon = geo.lon * TORAD;
    return xyzunit(xyz_t{
        (- EARTHR - geo.height) * cos(geo.lon) * sin(geo.lat),
        (- EARTHR - geo.height) * sin(geo.lon) * sin(geo.lat),
        
        ((EARTHR * EARTHECC2 * (-EARTHECC2 + 1) * sin(geo.lat) * sin(geo.lat) * cos(geo.lat))
        / pow(((-EARTHECC2 * sin(geo.lat) * sin(geo.lat)) + 1), 3.0f/2.0f))
        + ((((EARTHR * (-EARTHECC2 + 1))
        / sqrt((-EARTHECC2 * sin(geo.lat) * sin(geo.lat)) + 1)) + geo.height)
        * cos(geo.lat))

    });
}

inline xyz_t uLon(xyz_t geo) {
    geo.lat = geo.lat * TORAD;
    geo.lon = geo.lon * TORAD;
    return xyzunit(xyz_t{
        (- EARTHR - geo.height) * cos(geo.lat) * sin(geo.lon),
        (EARTHR + geo.height) * cos(geo.lat) * cos(geo.lon),
        0.0f
    });
}

inline xyz_t uVert(xyz_t geo) {
    xyz_t v = geoToECEF(geo);
    return xyzunit(v);
}
