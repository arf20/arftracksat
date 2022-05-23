#include "main.hpp"
#include "sat.hpp"
#include "graphics.hpp"

#include <ctime>
#include <thread>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <string>

#include <curl/curl.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

json config;

std::string tleroot;

station sta;

// sat filter and column filter
std::vector<std::string> show;
std::vector<std::vector<sat>::iterator> shownSats;
std::vector<std::string> columns;

bool validateConfig() {
#ifdef _WIN32
	constexpr auto confPath = "config.json";
#else
	constexpr auto confPath = "/usr/local/etc/arftracksat/config.json";
#endif
	if (!std::filesystem::exists(confPath)) {
		std::cout << "No config.json" << std::endl;
		return false;
	}

	std::ifstream confFile(confPath);
	try {
		confFile >> config;
	}
	catch (json::exception& e) {
		std::cout << "Malformed JSON conf: " + std::string(e.what()) << std::endl;
		return false;
	}

	if (!config.contains("tleroot")) { std::cout << "tleroot not defined in config" << std::endl; return false; }
	if (config["tleroot"].type() != json::value_t::string) { std::cout << "tleroot is not a string" << std::endl; return false; }
	tleroot = config["tleroot"];
	if (!std::filesystem::exists(tleroot)) { 
		std::cout << "tleroot does not exist, creating it: " << tleroot << std::endl;
		if (!std::filesystem::create_directory(tleroot)) { std::cout << "error creating directory. tleroot invalid" << std::endl; return false; }
	}

	if (!config.contains("updatePerdiod")) { std::cout << "updatePerdiod not defined in config" << std::endl; return false; }
	if (config["updatePerdiod"].type() != json::value_t::number_unsigned) { std::cout << "updatePerdiod is not a unsigned number" << std::endl; return false; }

	if (!config.contains("station")) { std::cout << "station not defined in config" << std::endl; return false; }
	if (config["station"].type() != json::value_t::object) { std::cout << "station is not a object" << std::endl; return false; }
	if (!config["station"].contains("name")) { std::cout << "name not defined in config" << std::endl; return false; }
	if (config["station"]["name"].type() != json::value_t::string) { std::cout << "name is not a string" << std::endl; return false; }
	if (!config["station"].contains("lat")) { std::cout << "lat not defined in config" << std::endl; return false; }
	if (config["station"]["lat"].type() != json::value_t::number_float) { std::cout << "lat is not a decimal number" << std::endl; return false; }
	if (!config["station"].contains("lon")) { std::cout << "lon not defined in config" << std::endl; return false; }
	if (config["station"]["lon"].type() != json::value_t::number_float) { std::cout << "lon is not a decimal number" << std::endl; return false; }
	if (!config["station"].contains("hgt")) { std::cout << "hgt not defined in config" << std::endl; return false; }
	if (config["station"]["hgt"].type() != json::value_t::number_float) { std::cout << "hgt is not a decimal number" << std::endl; return false; }

	if (!config.contains("mapfile")) { std::cout << "mapfile not defined in config" << std::endl; return false; }
	if (config["mapfile"].type() != json::value_t::string) { std::cout << "mapfile is not a string" << std::endl; return false; }
	std::string mapfile = config["mapfile"];
	if (!std::filesystem::exists(mapfile)) { 
		std::cout << "mapfile does not exist: " << mapfile << std::endl;
		return false;
	}

	return true;
}

bool checkTLE() {
	if (!config.contains("tlefile")) { std::cout << "tlefile not defined in config" << std::endl; return false; }
	if (config["tlefile"].type() != json::value_t::string) { std::cout << "tlefile is not a string" << std::endl; return false; }
	if (!std::filesystem::exists(tleroot + std::string(config["tlefile"]))) { std::cout << "tlefile is not a valid file" << std::endl; return false; }

	return true;
}

void getTLEs(std::string root, std::vector<std::string> urls) {
	curlpp::Cleanup cleanup;
	curlpp::Easy request;

	int i = 0;
	for (std::string& source : urls) {
		std::ofstream tlef(root + source.substr(source.find_last_of("/\\") + 1), std::ios::binary | std::ios::out);

		try {
			std::cout << "Get:" << i << " " << source << std::endl;
			request.setOpt<curlpp::Options::Url>(source);
			curlpp::options::WriteStream ws(&tlef);
			request.setOpt(ws);
			request.perform();
			i++;
		}
		catch (curlpp::RuntimeError e) { std::cout << e.what() << std::endl; }
		catch (curlpp::LogicError e) { std::cout << e.what() << std::endl; }

		tlef.close();
	}
}

int main(int argc, char **argv) {
	std::cout << "arftracksat by arf20" << std::endl;

	// check json
	if (!validateConfig()) exit(1);

	// get tle files from celestrak
	getTLEs(tleroot, config["tlesources"].get<std::vector<std::string>>());

	// check existance of tle
	if (!checkTLE()) exit(1);

	// directories
	std::string tlefn = tleroot + std::string(config["tlefile"]);
	loadSats(tlefn);

	// update period
	long period = config["updatePerdiod"];	

	// sat filter and column filter
	show = config["show"].get<std::vector<std::string>>();
	columns = config["columns"].get<std::vector<std::string>>();

	for (int i = 0; i < sats.size(); i++) {
		if (show.size() == 0) {															// if show empty, shove all in
			shownSats.push_back(sats.begin() + i);
		}
		else if (std::find(show.begin(), show.end(), sats[i].name) != show.end()) {		// if show not empty, shove show in
			shownSats.push_back(sats.begin() + i);
		} 
	}

	// if show was completely invalid, shove everything in
	if (shownSats.size() == 0) {
		for (int i = 0; i < sats.size(); i++) {
			shownSats.push_back(sats.begin() + i);
		}
	} 

	// set station data
	sta.name = config["station"]["name"];

	sta.geo.lat = config["station"]["lat"];
	sta.geo.lon = config["station"]["lon"];
	sta.geo.height = config["station"]["hgt"];

	xyz_geodetic_to_ecef(&sta.geo, &sta.pos);

	sta.vel.x = 0.0;
	sta.vel.y = 0.0;
	sta.vel.z = 0.0;

	std::cout << "Setup done, entering loop..." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	// Give user time to read

	// ========================= SETUP DONE =========================

	// start graphics
	std::thread graphicThread(startGraphics);
	graphicThread.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	// Give user time to read

	std::cout << std::setprecision(1) << std::fixed;	// 2 decimal digit precision

	// time structures
	tm *futctime = new tm;
	tm *floctime = new tm;

	// main screen loop
	while (true) {
		// clear screen
#ifdef _WIN32
		system("cls");
#else
		std::cout << "\x1B[2J\x1B[H";
#endif
		// now in unix time
		time_t utct;
		// propagate orbits for all sats for moment now
		computeSats(time(&utct));

		// print station data
		std::cout << "STATION\t\tLAT\tLON\tHGT\t\tX\tY\tZ" << std::endl;
		std::cout << sta.name << "\t"; if (sta.name.length() < 8) std::cout << "\t";
		std::cout << sta.geo.lat << "\t" << sta.geo.lon << "\t" << sta.geo.height << "\t\t";
		std::cout << sta.pos.x << "\t" << sta.pos.y << "\t" << sta.pos.z << "\t" << std::endl << std::endl;

		// get time structs in UTC and local
#ifdef _WIN32
		gmtime_s(futctime, &utct);
		localtime_s(floctime, &utct);
#else
		gmtime_r(&utct, futctime);
		localtime_r(&utct, floctime);
#endif

		// print time
		std::cout << "TIME\tUTC\t" << futctime->tm_hour << ":" << futctime->tm_min << ":" << futctime->tm_sec << std::endl
			<< "\tLOCAL\t" << floctime->tm_hour << ":" << floctime->tm_min << ":" << floctime->tm_sec << std::endl;

		// header lines
		std::string cl1 = "";
		std::string cl2 = "";

		// populate header
		for (std::string& col : columns) {
			if (col == "name") { cl2 += "NAME\t\t"; cl1 += "\t\t"; }
			if (col == "azel") { cl2 += "AZ\tEL\t"; cl1 += "\t\t"; }
			if (col == "dis") { cl2 += "DIS\t"; cl1 += "\t"; }
			if (col == "geo") { cl2 += "LAT\tLON\tHGT\t"; cl1 += "\t\t\t"; }
			if (col == "pos") { cl2 += "X\tY\tZ\t"; cl1 += "POS\t\t\t"; }
			if (col == "vel") { cl2 += "X\tY\tZ\t"; cl1 += "VEL\t\t\t"; }
			if (col == "tab") { cl2 += "\t"; cl1 += "\t"; }
		}

		// print header
		std::cout << cl1 << std::endl << cl2 << std::endl;

		// per sat loop
		for (int i = 0; i < shownSats.size(); i++) {
			sat& sat = *shownSats[i];

			// per column
			for (std::string& col : columns) {
				if (col == "name") {
					std::cout << sat.name << "\t";
					if (sat.name.length() < 8) std::cout << "\t";
				}
				if (col == "azel") std::cout << sat.aer.azimuth << "\t" << sat.aer.elevation << "\t";
				if (col == "dis") {
					std::cout << sat.aer.distance << "\t";
				}
				if (col == "geo") std::cout << sat.geo.lat << "\t" << sat.geo.lon << "\t" << sat.geo.height << "\t";
				if (col == "pos") {
					std::cout << sat.pos.x << "\t";
					std::cout << sat.pos.y << "\t";
					std::cout << sat.pos.z << "\t";
				}
				if (col == "vel") {
					std::cout << sat.vel.x << "\t";
					std::cout << sat.vel.y << "\t";
					std::cout << sat.vel.z << "\t";
				}
				if (col == "tab") std::cout << "\t";
			}

			// hardcoded doppler print for debug
			std::cout << "\t" << sat.dopShift << std::endl;
		}

		// sleep for period
		std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

	// never reached ;)
	exit(0);
}

