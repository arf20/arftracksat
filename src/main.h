#pragma once

#include <string>
#include <vector>
#include <cinttypes>

#include <nlohmann/json.hpp>
using namespace nlohmann;

struct vector {
	double v[3];

	double x() {
		return v[0];
	}

	void setx(double _x) {
		v[0] = _x;
	}

	double y() {
		return v[1];
	}

	void sety(double _y) {
		v[1] = _y;
	}

	double z() {
		return v[2];
	}

	void setz(double _z) {
		v[2] = _z;
	}

	vector operator+(vector _right) {
		return vector{ x() + _right.x(), y() + _right.y(), z() + _right.z() };
	}

	vector operator-(vector _right) {
		return vector{ x() - _right.x(), y() - _right.y(), z() - _right.z() };
	}

	vector operator/(double _right) {
		return vector{ x() / _right, y() / _right, z() / _right };
	}

	double mod() {
		return sqrt(pow(x(), 2) + pow(y(), 2) + pow(z(), 2));
	}

	double dot(vector _right) {
		return ((x() * _right.x()) + (y() * _right.y()) + (z() * _right.z()));
	}
};

struct latlonhgt {
	double v[3];

	double lat() {
		return v[0];
	}

	void setlat(double _lat) {
		v[0] = _lat;
	}

	double lon() {
		return v[1];
	}

	void setlon(double _lon) {
		v[1] = _lon;
	}

	double hgt() {
		return v[2];
	}

	void sethgt(double _hgt) {
		v[2] = _hgt;
	}
};

struct azelrd {
	double az;
	double el;
	double r;
	double gcd;
};

struct sat {
	std::string id;
	std::string name;
	int64_t key;
	vector pos;
	vector vel;
	latlonhgt llh;
	azelrd aerd;
	double rVel;
	double dopShift;
};

struct station {
	std::string name;
	vector pos;
	vector vel;
	latlonhgt llh;
};

#define EARTHR	6371.0
#define EQR		6378.0
#define POR		6356.8

#define LIGHTC	299792458.0

// main
extern json config;
extern station sta;

// SAT
extern std::vector<sat> sats;
void loadSats(std::string spg4libroot, std::string licfile, std::string tlefile);
void computeSats(time_t t);
vector llhToECEF(latlonhgt llh);