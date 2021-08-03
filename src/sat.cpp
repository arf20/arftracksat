#include "main.h"

extern "C" {
#include <sgp4/services/DllMainDll_Service.h>
#include <sgp4/services/TimeFuncDll_Service.h>
#include <sgp4/wrappers/DllMainDll.h>
#include <sgp4/wrappers/EnvConstDll.h>
#include <sgp4/wrappers/AstroFuncDll.h>
#include <sgp4/wrappers/TimeFuncDll.h>
#include <sgp4/wrappers/TleDll.h>
#include <sgp4/wrappers/Sgp4PropDll.h>
}

#include <ctime>
#include <iostream>

// Load all the dlls being used in the program
void LoadAstroStdDlls(const char* libPath) {
	// Load MainDll dll
	LoadDllMainDll(libPath);
	// Load EnvConst dll and assign function pointers
	LoadEnvConstDll(libPath);
	// Load TimeFunc dll and assign function pointers
	LoadTimeFuncDll(libPath);
	// Load AstroFunc dll and assign function pointers
	LoadAstroFuncDll(libPath);
	// Load Tle dll and assign function pointers
	LoadTleDll(libPath);
	// Load Sgp4Prop dll and assign function pointers
	LoadSgp4PropDll(libPath);
}


// Initialize all the dlls being used in the program
void InitAstroStdDlls() {
	fAddr apPtr;
	int errCode;

	// Get pointer to the global data (data pointers, function pointers, ...) that's being used among the dlls in the program
	apPtr = DllMainInit();

	errCode = EnvInit(apPtr);
	if (errCode != 0)
		ShowMsgAndTerminate();

	errCode = TimeFuncInit(apPtr);
	if (errCode != 0)
		ShowMsgAndTerminate();

	errCode = AstroFuncInit(apPtr);
	if (errCode != 0)
		ShowMsgAndTerminate();

	errCode = TleInit(apPtr);
	if (errCode != 0)
		ShowMsgAndTerminate();

	errCode = Sgp4Init(apPtr);
	if (errCode != 0)
		ShowMsgAndTerminate();
}


// Free all the dlls being used in the program
void FreeAstroStdDlls() {
	// Free MainDll dll
	FreeDllMainDll();
	// Free EnvConst dll
	FreeEnvConstDll();
	// Free AstroFunc dll
	FreeAstroFuncDll();
	// Free TimeFunc dll
	FreeTimeFuncDll();
	// Free Tle dll
	FreeTleDll();
	// Free Sgp4Prop dll
	FreeSgp4PropDll();
}

std::vector<sat> sats;

std::vector<std::pair<std::string, std::string>> idnames;

void loadSatIdNames() {
	for (sat& sat : sats) {
		//if (!config.contains(sat.id)) continue;
		try {
			idnames.push_back({ sat.id, config["satidnames"][sat.id] });
		}
		catch (nlohmann::json::exception e) {
			std::cout << "Warning: No name for ID " << sat.id << std::endl;
		}
	}
}

std::string id2name(std::string id) {
	for (std::pair<std::string, std::string>& pair : idnames) {
		if (pair.first == id) return pair.second;
	}
	return "Unknown";
}

// geodetic to ECEF
vector llhToECEF(latlonhgt llh) {
	double e2 = 1 - (pow(POR, 2) / pow(EQR, 2));
	//double N = pow(EQR, 2) / sqrt((pow(EQR, 2) * pow(cos(llh.lat()), 2)) + (pow(POR, 2) * pow(sin(llh.lat()), 2)));
	double N = EQR / sqrt(1 - (e2 * pow(sin(llh.lat()), 2)));

	vector v;
	v.setx((N + llh.hgt()) * cos(TORAD * llh.lat()) * cos(TORAD * llh.lon()));					// X
	v.sety((N + llh.hgt()) * cos(TORAD * llh.lat()) * sin(TORAD * llh.lon()));					// Y
	//v.setz((((pow(POR, 2) / pow(EQR, 2)) * N) + llh.hgt()) * sin(TORAD *llh.lat()));			// Z
	v.setz((((1 - e2) * N) + llh.hgt()) * sin(TORAD * llh.lat()));								// Z
	return v;
}

void loadSats(std::string spg4libroot, std::string licpath, std::string tlefile) {
	// Load DLLs
	LoadAstroStdDlls(spg4libroot.c_str());
	// Check license
	Sgp4SetLicFilePath((char*)licpath.c_str());
	// Init DLLs
	InitAstroStdDlls();

	// Get DLL info
	char infoStr[128];
	Sgp4GetInfo(infoStr);
	infoStr[INFOSTRLEN - 1] = 0;					// Insert NUL

	std::cout << infoStr << std::endl;

	auto tlefilecstr = (char*)tlefile.c_str();
	if (TleLoadFile(tlefilecstr)) {	// load TLE
		std::cout << "Error opening TLE file" << std::endl;
		exit(0);
	}
	/*if (TConLoadFile((char*)tlefn.c_str())) {		// load TLE 2 (6P card)
		std::cout << "Error opening TLE file" << std::endl;
		exit(0);
	}*/

	__int64 *pSatKeys;

	int numSats = TleGetCount();
	pSatKeys = (__int64*)malloc(numSats * sizeof(__int64));	// array with sat keys
	TleGetLoaded(2, pSatKeys);						// load sat keys

	//std::cout << "Satellites:" << std::endl;

	for (int i = 0; i < numSats; i++) {
		sat s;

		if (Sgp4InitSat(pSatKeys[i]) != 0)
			ShowMsgAndTerminate();

		char satName[512];
		memset(satName, 0, GETSETSTRLEN);
		TleGetField(pSatKeys[i], XF_TLE_SATNAME, satName);
		int j = GETSETSTRLEN - 1;
		while (satName[j] == ' ') {
			j--;
		}
		satName[j] = 0;
		s.id = std::string(satName);
		s.key = pSatKeys[i];

		sats.push_back(s);

		//std::cout << "\t" + s.name << std::endl;
	}

	loadSatIdNames();

	for (sat& sat : sats) {
		sat.name = id2name(sat.id);
	}

	std::cout << "Satellites loaded" << std::endl;
}

void computeSats(time_t t) {
	double d50 = (t / (60.0 * 60.0 * 24.0)) + 7306.0;	// days since 1950 UTC
	//std::cout << "Days since 1950: " << d50 << std::endl << std::endl;

	for (sat& sat : sats) {
		double mse;
		int errCode = Sgp4PropDs50UTC(sat.key, d50, &mse, sat.pos.v, sat.vel.v, sat.llh.v);	// propagate

		while (sat.llh.lon() < -180.0)
			sat.llh.setlon(sat.llh.lon() + 360.0);

		while (sat.llh.lon() > (180.0))
			sat.llh.setlon(sat.llh.lon() - 360.0);

		if (errCode != 0) {
			char errMsg[128];
			GetLastErrMsg(errMsg);
			errMsg[LOGMSGLEN - 1] = 0;

			// Display error message to screen
			printf("%s\n", errMsg);

			continue; // Move to the next satellite
		}

		// == AZ ==
		// Difference in coords
		double deltaLat = sat.llh.lat() - sta.llh.lat();
		double deltaLon = sat.llh.lon() - sta.llh.lon();

		// Formulae for azimuth
		sat.aerd.az = TODEG * atan2(
			sin(TORAD * deltaLon) * cos(TORAD * sat.llh.lat()),
			(cos(TORAD * sta.llh.lat()) * sin(TORAD * sat.llh.lat()) - (sin(TORAD * sta.llh.lat()) * cos(TORAD * sat.llh.lat()) * cos(TORAD * deltaLon)))
		);

		// Correct negative azimuths
		while (sat.aerd.az < 0.0)
			sat.aerd.az += 360.0;


		// == EL ==
		// Haversine formulae
		double h = pow(sin((TORAD * deltaLat) / 2.0), 2)
			+ (cos(TORAD * sta.llh.lat()) * cos(TORAD * sat.llh.lat()) * pow(sin((TORAD * deltaLon) / 2.0), 2));	// haversine of theta
		double theta = 2.0 * atan2(sqrt(h), sqrt(1.0 - h));															// arc angle between 2 coords (rad)
		double gcd = EARTHR * theta;																				// arc length

		// Triangle:  earth center (C), observer (O) and satellite (S)
		double oRLen = EARTHR + sta.llh.hgt();																		// CO length
		double sRLen = EARTHR + sat.llh.hgt();																		// CS length
			
		double osLen = sqrt(pow(oRLen, 2) + pow(sRLen, 2) - (2 * oRLen * sRLen * cos(theta)));						// OS length (missing side)

		// Find CO OS angle
		double phi = asin((sRLen * sin(theta)) / osLen);

		// Elevation is that angle minus 90º
		sat.aerd.el = (TODEG * phi) - 90;



		// Radius (range) to sat
		sat.aerd.r = osLen;
		// Great circle distance
		sat.aerd.gcd = gcd;


		sat.rVel = (sat.vel - sta.vel).dot((sat.pos - sta.pos) / (sat.pos - sta.pos).mod());
		sat.dopShift = (sat.rVel / LIGHTC) * 137500000.0;
	}
}