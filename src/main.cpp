#include "../common/sgdp4/sgdp4.h"
#include "../core/sat.hpp"
#include "../representation/graphics.hpp"

#include <QApplication>
#include "mainwindow.h"

#include <curl/curl.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <ctime>
#include <thread>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <cinttypes>

#include <nlohmann/json.hpp>
using namespace nlohmann;

json config;

static std::string tleroot;

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

	if (!config.contains("objfile")) { std::cout << "objfile not defined in config" << std::endl; return false; }
	if (config["objfile"].type() != json::value_t::string) { std::cout << "objfile is not a string" << std::endl; return false; }
	std::string objfile = config["objfile"];
	if (!std::filesystem::exists(objfile)) { 
		std::cout << "objfile does not exist: " << objfile << std::endl;
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

	bool noGraphics = false;
	bool noDownload = false;
	bool noPrint = false;
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == "--no-graphics") noGraphics = true;
		if (std::string(argv[i]) == "--no-download") noDownload = true;
		if (std::string(argv[i]) == "--no-print") noPrint = true;

	}

	// check json
	if (!validateConfig()) exit(1);

	// get tle files from celestrak
	if (!noDownload)
		getTLEs(tleroot, config["tlesources"].get<std::vector<std::string>>());

	// check existance of tle
	if (!checkTLE()) exit(1);

	// directories
	std::string tlefn = tleroot + std::string(config["tlefile"]);
	loadSats(tlefn);

	// update period
	long period = config["updatePerdiod"];	

	// sat filter and column filter
	std::vector<std::string> show = config["show"].get<std::vector<std::string>>();
	std::vector<std::string> columns = config["columns"].get<std::vector<std::string>>();
	std::vector<std::vector<sat>::iterator> shownSats;

	if (sats.size() == 0) {
		std::cout << "No sats" << std::endl;
		exit(1);
	}

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
	station sta;
	sta.name = config["station"]["name"];

	sta.geo.lat = config["station"]["lat"];
	sta.geo.lon = config["station"]["lon"];
	sta.geo.height = float(config["station"]["hgt"]) / 1000.0f;

	xyz_t sta_geo_rad = sta.geo;
	sta_geo_rad.lat *= TORAD;
	sta_geo_rad.lon *= TORAD;
	xyz_geodetic_to_ecef(&sta_geo_rad, &sta.pos);

	sta.vel.x = 0.0;
	sta.vel.y = 0.0;
	sta.vel.z = 0.0;

	std::string mapfile = std::string(config["mapfile"]);
	std::string objfile = std::string(config["objfile"]);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	// Give user time to read

	// ========================= SETUP DONE =========================

    // start Qt main window
    QApplication app(argc, argv);
    MainWindow mainwindow;

    mainwindow.show();
    app.exec();

	// never reached ;)
	exit(0);
}
