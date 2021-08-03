// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#ifndef ASTROFUNCDLL_H
#define ASTROFUNCDLL_H

#include "../services/DllUtils.h"

// Provide the path to the dll/so
#ifdef _WIN32
  #define AstroFuncDll "AstroFunc.dll"
#else
  #define AstroFuncDll "libastrofunc.so"
#endif


// Initializes AstroFunc DLL for use in the program.
// If this function returns an error, it is recommended that you stop the program immediately.
// 
// An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
// apPtr              The handle that was returned from DllMainInit(). See the documentation for DllMain.dll for details. (in-Long)
// returns 0 if AstroFunc.dll is initialized successfully, non-0 if there is an error.
typedef int (STDCALL *fnPtrAstroFuncInit)(__int64 apPtr);


// Retrieves information about the current version of AstroFunc.dll. The information is placed in the string parameter you pass in.
// The returned string provides information about the version number, build date, and platform.
// infoStr            A string to hold the information about AstroFunc.dll. (out-Character[128])
typedef void (STDCALL *fnPtrAstroFuncGetInfo)(char infoStr[128]);


// Converts a set of Keplerian elements to a set of equinoctial elements. 
// metricKep          The set of Keplerian elements to be converted. (in-Double[6])
// metricEqnx         The resulting set of equinoctial elements. (out-Double[6])
typedef void (STDCALL *fnPtrKepToEqnx)(double metricKep[6], double metricEqnx[6]);


// Converts a set of osculating Keplerian elements to osculating position and velocity vectors.
// metricKep          The set of Keplerian elements to be converted. (in-Double[6])
// pos                The resulting position vector. (out-Double[3])
// vel                The resulting velocity vector. (out-Double[3])
typedef void (STDCALL *fnPtrKepToPosVel)(double metricKep[6], double pos[3], double vel[3]);


// Converts a set of Keplerian elements to Ubar, Vbar, and Wbar vectors.
// metricKep          The set of Keplerian elements to be converted. (in-Double[6])
// uBar               The resulting ubar vector. (out-Double[3])
// vBar               The resulting vbar vector. (out-Double[3])
// wBar               The resulting wbar vector. (out-Double[3])
typedef void (STDCALL *fnPtrKepToUVW)(double metricKep[6], double uBar[3], double vBar[3], double wBar[3]);


// Converts a set of classical elements to a set of equinoctial elements. 
// metricClass        The set of classical elements to be converted. (in-Double[6])
// metricEqnx         The resulting set of equinoctial elements. (out-Double[6])
typedef void (STDCALL *fnPtrClassToEqnx)(double metricClass[6], double metricEqnx[6]);


// Converts a set of equinoctial elements to a set of classical elements.
// metricEqnx         The set of equinoctial elements to be converted. (in-Double[6])
// metricClass        The resulting set of classical elements. (out-Double[6])
typedef void (STDCALL *fnPtrEqnxToClass)(double metricEqnx[6], double metricClass[6]);


// Converts a set of equinoctial elements to a set of Keplerian elements. 
// metricEqnx         The set of equinoctial elements to be converted. (in-Double[6])
// metricKep          The resulting set of Keplerian elements. (out-Double[6])
typedef void (STDCALL *fnPtrEqnxToKep)(double metricEqnx[6], double metricKep[6]);


// Converts a set of equinoctial elements to position and velocity vectors.
// metricEqnx         The set of equinoctial elements to be converted. (in-Double[6])
// pos                The resulting position vector. (out-Double[3])
// vel                The resulting velocity vector. (out-Double[3])
typedef void (STDCALL *fnPtrEqnxToPosVel)(double metricEqnx[6], double pos[3], double vel[3]);


// Converts position and velocity vectors to a set of equinoctial elements.
// pos                The position vector to be converted. (in-Double[3])
// vel                The velocity vector to be converted. (in-Double[3])
// metricEqnx         The resulting set of equinoctial elements. (out-Double[6])
typedef void (STDCALL *fnPtrPosVelToEqnx)(double pos[3], double vel[3], double metricEqnx[6]);


// Converts position and velocity vectors to a set of equinoctial elements with the given mu value. 
// This function is used when working with the SP propagator to get a more accurate set of equinoctial elements.
// pos                The position vector to be converted. (in-Double[3])
// vel                The velocity vector to be converted. (in-Double[3])
// mu                 The value of mu. (in-Double)
// metricEqnx         The resulting set of equinoctial elements. (out-Double[6])
typedef void (STDCALL *fnPtrPosVelMuToEqnx)(double pos[3], double vel[3], double mu, double metricEqnx[6]);


// Converts osculating position and velocity vectors to a set of osculating Keplerian elements.
// pos                The position vector to be converted. (in-Double[3])
// vel                The velocity vector to be converted. (in-Double[3])
// metricKep          The resulting set of Keplerian elements. (out-Double[6])
typedef void (STDCALL *fnPtrPosVelToKep)(double pos[3], double vel[3], double metricKep[6]);


// Converts osculating position and velocity vectors to a set of osculating Keplerian elements with the given value of mu.
// This function is used when working with the SP propagator to get a more accurate set of Keplerian elements.
// pos                The position vector to be converted. (in-Double[3])
// vel                The velocity vector to be converted. (in-Double[3])
// mu                 The value of mu. (in-Double)
// metricKep          The resulting set of Keplerian elements. (out-Double[6])
typedef void (STDCALL *fnPtrPosVelMuToKep)(double pos[3], double vel[3], double mu, double metricKep[6]);


// Converts position and velocity vectors to U, V, W vectors. See the remarks section for details.
// The resulting vectors have the following meanings.
// U vector: along radial direction
// V vector: W x U
// W vector: pos x vel
// pos                The position vector to be converted. (in-Double[3])
// vel                The velocity vector to be converted. (in-Double[3])
// uVec               The resulting U vector. (out-Double[3])
// vVec               The resulting V vector. (out-Double[3])
// wVec               The resulting W vector. (out-Double[3])
typedef void (STDCALL *fnPtrPosVelToUUVW)(double pos[3], double vel[3], double uVec[3], double vVec[3], double wVec[3]);


// Converts position and velocity vectors to U, V, W vectors. See the remarks section for details.
// The resulting vectors have the following meanings.
// U vector: V x W
// V vector: along velocity direction
// W vector: pos x vel
// pos                The position vector. (in-Double[3])
// vel                The velocity vector. (in-Double[3])
// uVec               The resulting U vector. (out-Double[3])
// vVec               The resulting V vector. (out-Double[3])
// wVec               The resulting W vector. (out-Double[3])
typedef void (STDCALL *fnPtrPosVelToPTW)(double pos[3], double vel[3], double uVec[3], double vVec[3], double wVec[3]);


// Solves Kepler's equation (M = E - e sin(E)) for the eccentric anomaly, E, by iteration.
// metricKep          The set of Keplerian elements for which to solve the equation. (in-Double[6])
// returns The eccentric anomaly.
typedef double (STDCALL *fnPtrSolveKepEqtn)(double metricKep[6]);


// Computes true anomaly from a set of Keplerian elements.
// metricKep          The set of Keplerian elements for which to compute true anomaly. (in-Double[6])
// returns The true anomaly in degrees.
typedef double (STDCALL *fnPtrCompTrueAnomaly)(double metricKep[6]);


// Converts mean motion N to semi-major axis A.
// n                  Mean motion N (revs/day). (in-Double)
// returns The semi-major axis A (km).
typedef double (STDCALL *fnPtrNToA)(double n);


// Converts semi-major axis A to mean motion N.
// a                  Semi-major axis A (km). (in-Double)
// returns The mean motion N (revs/day).
typedef double (STDCALL *fnPtrAToN)(double a);


// Converts Kozai mean motion to Brouwer mean motion.
// SGP TLE's use Kozai mean motion while SGP4/SGP4-XP TLE's use Brouwer mean motion.
// eccen              eccentricity (in-Double)
// incli              inclination (degrees) (in-Double)
// nKozai             Kozai mean motion (revs/day). (in-Double)
// returns Brouwer mean motion (revs/day).
typedef double (STDCALL *fnPtrKozaiToBrouwer)(double eccen, double incli, double nKozai);


// Converts Brouwer mean motion to Kozai mean motion.
// SGP TLE's use Kozai mean motion while SGP4/SGP4-XP TLE's use Brouwer mean motion.
// eccen              eccentricity (in-Double)
// incli              inclination (degrees) (in-Double)
// nBrouwer           Brouwer mean motion (revs/day). (in-Double)
// returns Kozai mean motion (revs/day).
typedef double (STDCALL *fnPtrBrouwerToKozai)(double eccen, double incli, double nBrouwer);


// Converts a set of osculating Keplerian elements to a set of mean Keplerian elements using method 9 algorithm.
// metricOscKep       The set of osculating Keplerian elements to be converted. (in-Double[6])
// metricMeanKep      The resulting set of mean Keplerian elements. (out-Double[6])
typedef void (STDCALL *fnPtrKepOscToMean)(double metricOscKep[6], double metricMeanKep[6]);


// Converts an ECI position vector XYZ to geodetic latitude, longitude, and height.
// thetaG             ThetaG - Greenwich mean sidereal time (rad). (in-Double)
// metricPos          The ECI (TEME of Date) position vector (km) to be converted. (in-Double[3])
// metricLLH          The resulting geodetic north latitude (degree), east longitude(degree), and height (km). (out-Double[3])
typedef void (STDCALL *fnPtrXYZToLLH)(double thetaG, double metricPos[3], double metricLLH[3]);


// Converts an ECI position vector XYZ to geodetic latitude, longitude, and height at the specified time.
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// metricPos          The ECI (TEME of Date) position vector (km) to be converted. (in-Double[3])
// metricLLH          The resulting geodetic north latitude (degree), east longitude(degree), and height (km). (out-Double[3])
typedef void (STDCALL *fnPtrXYZToLLHTime)(double ds50UTC, double metricPos[3], double metricLLH[3]);


// Converts geodetic latitude, longitude, and height to an ECI position vector XYZ.
// thetaG             Theta - Greenwich mean sidereal time (rad). (in-Double)
// metricLLH          An array containing geodetic north latitude (degree), east longitude (degree), and height (km) to be converted. (in-Double[3])
// metricXYZ          The resulting ECI (TEME of Date) position vector (km). (out-Double[3])
typedef void (STDCALL *fnPtrLLHToXYZ)(double thetaG, double metricLLH[3], double metricXYZ[3]);


// Converts geodetic latitude, longitude, and height to an ECI position vector XYZ at the specified time.
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// metricLLH          An array containing geodetic north latitude (degree), east longitude (degree), and height (km) to be converted. (in-Double[3])
// metricXYZ          The resulting ECI (TEME of Date) position vector (km). (out-Double[3])
typedef void (STDCALL *fnPtrLLHToXYZTime)(double ds50UTC, double metricLLH[3], double metricXYZ[3]);


// Converts EFG position and velocity vectors to ECI position and velocity vectors.
// thetaG             Theta - Greenwich mean sidereal time (rad). (in-Double)
// posEFG             The EFG position vector (km) to be converted. (in-Double[3])
// velEFG             The EFG velocity vector (km/s) to be converted. (in-Double[3])
// posECI             The resulting ECI (TEME of Date) position vector (km). (out-Double[3])
// velECI             The resulting ECI (TEME of Date) velocity vector (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrEFGToECI)(double thetaG, double posEFG[3], double velEFG[3], double posECI[3], double velECI[3]);


// Converts EFG position and velocity vectors to ECI position and velocity vectors at the specified time.
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// posEFG             The EFG position vector (km) to be converted. (in-Double[3])
// velEFG             The EFG velocity vector (km/s) to be converted. (in-Double[3])
// posECI             The resulting ECI (TEME of Date) position vector (km). (out-Double[3])
// velECI             The resulting ECI (TEME of Date) velocity vector (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrEFGToECITime)(double ds50UTC, double posEFG[3], double velEFG[3], double posECI[3], double velECI[3]);


// Converts ECI position and velocity vectors to EFG position and velocity vectors.
// thetaG             Theta - Greenwich mean sidereal time (rad). (in-Double)
// posECI             The ECI (TEME of Date) position vector (km) to be converted. (in-Double[3])
// velECI             The ECI (TEME of Date) velocity vector (km/s) to be converted. (in-Double[3])
// posEFG             The resulting EFG position vector (km). (out-Double[3])
// velEFG             The resulting EFG velocity vector (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrECIToEFG)(double thetaG, double posECI[3], double velECI[3], double posEFG[3], double velEFG[3]);


// Converts ECI position and velocity vectors to EFG position and velocity vectors at the specified time.
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// posECI             The ECI (TEME of Date) position vector (km) to be converted. (in-Double[3])
// velECI             The ECI (TEME of Date) velocity vector (km/s) to be converted. (in-Double[3])
// posEFG             The resulting EFG position vector (km). (out-Double[3])
// velEFG             The resulting EFG velocity vector (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrECIToEFGTime)(double ds50UTC, double posECI[3], double velECI[3], double posEFG[3], double velEFG[3]);


// Converts ECR position and velocity vectors to EFG position and velocity vectors.
// polarX             Polar motion X (arc-sec). (in-Double)
// polarY             Polar motion Y (arc-sec). (in-Double)
// posECR             The ECR position vector (km) to be converted. (in-Double[3])
// velECR             The ECR velocity vector (km/s) to be converted. (in-Double[3])
// posEFG             The resulting EFG position vector (km). (out-Double[3])
// velEFG             The resulting EFG velocity vector (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrECRToEFG)(double polarX, double polarY, double posECR[3], double velECR[3], double posEFG[3], double velEFG[3]);


// Converts ECR position and velocity vectors to EFG position and velocity vectors at the specified time.
// ds50UTC            Input time in days since 1950 UTC. (in-Double)
// posECR             The ECR position vector (km) to be converted. (in-Double[3])
// velECR             The ECR velocity vector (km/s) to be converted. (in-Double[3])
// posEFG             The resulting EFG position vector (km) (set to zeros if timing constants not loaded). (out-Double[3])
// velEFG             The resulting EFG velocity vector (km/s) (set to zeros if timing constants not loaded). (out-Double[3])
typedef void (STDCALL *fnPtrECRToEFGTime)(double ds50UTC, double posECR[3], double velECR[3], double posEFG[3], double velEFG[3]);


// Converts EFG position and velocity vectors to ECR position and velocity vectors.
// polarX             Polar motion X (arc-sec). (in-Double)
// polarY             Polar motion Y (arc-sec). (in-Double)
// posEFG             The EFG position vector (km) to be converted. (in-Double[3])
// velEFG             The EFG velocity vector (km/s) to be converted. (in-Double[3])
// posECR             The resulting ECR position vector (km). (out-Double[3])
// velECR             The resulting ECR velocity vector (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrEFGToECR)(double polarX, double polarY, double posEFG[3], double velEFG[3], double posECR[3], double velECR[3]);


// Converts EFG position and velocity vectors to ECR position and velocity vectors at the specified time.
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// posEFG             The EFG position vector (km) to be converted. (in-Double[3])
// velEFG             The EFG velocity vector (km/s) to be converted. (in-Double[3])
// posECR             The resulting ECR position vector (km) (set to zeros if timing constants not loaded). (out-Double[3])
// velECR             The resulting ECR velocity vector (km/s) (set to zeros if timing constants not loaded). (out-Double[3])
typedef void (STDCALL *fnPtrEFGToECRTime)(double ds50UTC, double posEFG[3], double velEFG[3], double posECR[3], double velECR[3]);


// Converts an EFG position vector to geodetic latitude, longitude, and height.
// posEFG             The EFG position vector (km) to be converted. (in-Double[3])
// metricLLH          The resulting geodetic north latitude (degree), east longitude (degree), and height (km). (out-Double[3])
typedef void (STDCALL *fnPtrEFGPosToLLH)(double posEFG[3], double metricLLH[3]);


// Converts geodetic latitude, longitude, and height to an EFG position vector.
// metricLLH          An Array containing the geodetic north latitude (degree), east longitude (degree), and height (km) to be converted. (in-Double[3])
// posEFG             The resulting EFG position vector (km). (out-Double[3])
typedef void (STDCALL *fnPtrLLHToEFGPos)(double metricLLH[3], double posEFG[3]);


// Rotates position and velocity vectors from J2000 to coordinates of the specified date, expressed in ds50TAI.
// spectr             Specifies whether to run in SPECTR compatibility mode. A value of 1 means Yes. (in-Integer)
// nutationTerms      Nutation terms (4-106, 4:least accurate, 106:most acurate). (in-Integer)
// ds50TAI            The date to rotate to coordinates of, expressed in days since 1950, TAI. (in-Double)
// posJ2K             The position vector from J2000. (in-Double[3])
// velJ2K             The velocity vector from J2000. (in-Double[3])
// posDate            The resulting position vector in coordinates of date, ds50TAI. (out-Double[3])
// velDate            The resulting velocity vector in coordinates of date, ds50TAI. (out-Double[3])
typedef void (STDCALL *fnPtrRotJ2KToDate)(int spectr, int nutationTerms, double ds50TAI, double posJ2K[3], double velJ2K[3], double posDate[3], double velDate[3]);


// Rotates position and velocity vectors from coordinates of date to J2000.
// spectr             Specifies whether to run in SPECTR compatibility mode. A value of 1 means Yes. (in-Integer)
// nutationTerms      Nutation terms (4-106, 4:least accurate, 106:most acurate). (in-Integer)
// ds50TAI            Time in days since 1950, TAI for which the coordinates of position and velocity vectors are currently expressed. (in-Double)
// posDate            The position vector from coordinates of Date. (in-Double[3])
// velDate            The velocity vector from coordinates of Date. (in-Double[3])
// posJ2K             The resulting position vector in coordinates of J2000. (out-Double[3])
// velJ2K             The resulting velocity vector in coordinates of J2000. (out-Double[3])
typedef void (STDCALL *fnPtrRotDateToJ2K)(int spectr, int nutationTerms, double ds50TAI, double posDate[3], double velDate[3], double posJ2K[3], double velJ2K[3]);


// Computes the Sun and Moon position at the specified time.
// ds50ET             The number of days since 1950, ET for which to compute the sun and moon position. (in-Double)
// uvecSun            The resulting sun position unit vector. (out-Double[3])
// sunVecMag          The resulting magnitude of the sun position vector (km). (out-Double)
// uvecMoon           The resulting moon position unit vector. (out-Double[3])
// moonVecMag         The resulting magnitude of the moon position vector (km). (out-Double)
typedef void (STDCALL *fnPtrCompSunMoonPos)(double ds50ET, double uvecSun[3], double* sunVecMag, double uvecMoon[3], double* moonVecMag);


// Computes the Sun position at the specified time.
// ds50ET             The number of days since 1950, ET for which to compute the sun position. (in-Double)
// uvecSun            The resulting sun position unit vector. (out-Double[3])
// sunVecMag          The resulting magnitude of the sun position vector (km). (out-Double)
typedef void (STDCALL *fnPtrCompSunPos)(double ds50ET, double uvecSun[3], double* sunVecMag);


// Computes the Moon position at the specified time.
// ds50ET             The number of days since 1950, ET for which to compute the moon position. (in-Double)
// uvecMoon           The resulting moon position unit vector. (out-Double[3])
// moonVecMag         The resulting magnitude of the moon position vector (km). (out-Double)
typedef void (STDCALL *fnPtrCompMoonPos)(double ds50ET, double uvecMoon[3], double* moonVecMag);


// This function is intended for future use.  No information is currently available.
// This function is intended for future use.  No information is currently available.
// xf_Conv            Index of the conversion function (in-Integer)
// frArr              The input array (in-Double[128])
// toArr              The resulting array (out-Double[128])
typedef void (STDCALL *fnPtrAstroConvFrTo)(int xf_Conv, double frArr[128], double toArr[128]);


// Converts right ascension and declination to vector triad LAD in topocentric equatorial coordinate system.
// RA                 Right ascension (deg). (in-Double)
// dec                Declination (deg). (in-Double)
// L                  The resulting unit vector from the station to the satellite (referred to the equatorial coordinate system axis). (out-Double[3])
// A_Tilde            The resulting unit vector perpendicular to the hour circle passing through the satellite, in the direction of increasing RA. (out-Double[3])
// D_Tilde            The resulting unit vector perpendicular to L and is directed toward the north, in the plane of the hour circle. (out-Double[3])
typedef void (STDCALL *fnPtrRADecToLAD)(double RA, double dec, double L[3], double A_Tilde[3], double D_Tilde[3]);


// Converts azimuth and elevation to vector triad LAD in topocentric horizontal coordinate system.
// az                 Input azimuth (deg). (in-Double)
// el                 Input elevation angle (deg). (in-Double)
// Lh                 The resulting unit vector from the station to the satellite (referred to the horizon coordinate system axis). (out-Double[3])
// Ah                 The resulting unit vector perpendicular to the hour circle passing through the satellite, in the direction of increasing Az. (out-Double[3])
// Dh                 The resulting unit vector perpendicular to L and is directed toward the zenith, in the plane of the hour circle. (out-Double[3])
typedef void (STDCALL *fnPtrAzElToLAD)(double az, double el, double Lh[3], double Ah[3], double Dh[3]);


// Converts satellite ECI position/velocity vectors and sensor location to topocentric components.
// The xa_topo array has the following structure:
// [0]: Resulting right ascension (RA) (deg)
// [1]: Declination (deg)
// [2]: Azimuth (deg)
// [3]: Elevation (deg)
// [4]: Range (km)
// [5]: RAdot (first derivative of right ascension) (deg/s)
// [6]: DecDot (first derivative of declination) (deg/s)
// [7]: AzDot (first derivative of azimuth) (deg/s)
// [8]: ElDot (first derivative of elevation) (deg/s)
// [9]: RangeDot (first derivative of range) (km/s)   
// theta              Theta - local sidereal time(rad). (in-Double)
// lat                Station's astronomical latitude (deg). (+N) (-S) (in-Double)
// senPos             Sensor position in ECI (km). (in-Double[3])
// satPos             Satellite position in ECI (km). (in-Double[3])
// satVel             Satellite velocity in ECI (km/s). (in-Double[3])
// xa_topo            An array that stores the resulting topocentric components. (out-Double[10])
typedef void (STDCALL *fnPtrECIToTopoComps)(double theta, double lat, double senPos[3], double satPos[3], double satVel[3], double xa_topo[10]);


// Converts right ascension and declination in the topocentric reference frame to Azimuth/Elevation in the local horizon reference frame.
// thetaG             Theta - Greenwich mean sidereal time (rad). (in-Double)
// lat                Station's astronomical latitude (deg). (+N) (-S) (in-Double)
// lon                Station's astronomical longitude (deg). (+E) (-W) (in-Double)
// RA                 Right ascension (deg) (in-Double)
// dec                Declination (deg) (in-Double)
// az                 Azimuth (deg) (out-Double)
// el                 Elevation (deg) (out-Double)
typedef void (STDCALL *fnPtrRaDecToAzEl)(double thetaG, double lat, double lon, double RA, double dec, double* az, double* el);


// Converts right ascension and declination in the topocentric reference frame to Azimuth/Elevation in the local horizon reference frame.
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// lat                Station's astronomical latitude (deg). (+N) (-S) (in-Double)
// lon                Station's astronomical longitude (deg). (+E) (-W) (in-Double)
// RA                 Right ascension (deg) (in-Double)
// dec                Declination (deg) (in-Double)
// az                 Azimuth (deg) (out-Double)
// el                 Elevation (deg) (out-Double)
typedef void (STDCALL *fnPtrRaDecToAzElTime)(double ds50UTC, double lat, double lon, double RA, double dec, double* az, double* el);


// Converts Azimuth/Elevation in the local horizon reference frame to Right ascension/Declination in the topocentric reference frame
// thetaG             Theta - Greenwich mean sidereal time (rad). (in-Double)
// lat                Station's astronomical latitude (deg). (+N) (-S) (in-Double)
// lon                Station's astronomical longitude (deg). (+E) (-W) (in-Double)
// az                 Azimuth (deg) (in-Double)
// el                 Elevation (deg) (in-Double)
// RA                 Right ascension (deg) (out-Double)
// dec                Declination (deg) (out-Double)
typedef void (STDCALL *fnPtrAzElToRaDec)(double thetaG, double lat, double lon, double az, double el, double* RA, double* dec);


// Converts Azimuth/Elevation in the local horizon reference frame to Right ascension/Declination in the topocentric reference frame
// ds50UTC            Input time in days since 1950 UTC (in-Double)
// lat                Station's astronomical latitude (deg). (+N) (-S) (in-Double)
// lon                Station's astronomical longitude (deg). (+E) (-W) (in-Double)
// az                 Azimuth (deg) (in-Double)
// el                 Elevation (deg) (in-Double)
// RA                 Right ascension (deg) (out-Double)
// dec                Declination (deg) (out-Double)
typedef void (STDCALL *fnPtrAzElToRaDecTime)(double ds50UTC, double lat, double lon, double az, double el, double* RA, double* dec);


// Converts full state RAE (range, az, el, and their rates) to full state ECI (position and velocity)
// The xa_rae array has the following structure:
// [0]: Range (km)
// [1]: Azimuth (deg)
// [2]: Elevation (deg)
// [3]: Range Dot (km/s)
// [4]: Azimuth Dot (deg/s)
// [5]: Elevation Dot (deg/s)
// theta              Theta - local sidereal time(rad). (in-Double)
// astroLat           Astronomical latitude (ded). (in-Double)
// xa_rae             An array contains input data. (in-Double[6])
// senPos             Sensor position in ECI (km). (in-Double[3])
// satPos             Satellite position in ECI (km). (out-Double[3])
// satVel             Satellite velocity in ECI (km/s). (out-Double[3])
typedef void (STDCALL *fnPtrRAEToECI)(double theta, double astroLat, double xa_rae[6], double senPos[3], double satPos[3], double satVel[3]);


// Computes initial values for the SGP drag term NDOT and the SGP4 drag term BSTAR based upon eccentricity and semi-major axis.
// semiMajorAxis      Semi-major axis (km). (in-Double)
// eccen              Eccentricity (unitless). (in-Double)
// ndot               Ndot (revs/day^2). (out-Double)
// bstar              Bstar (1/earth radii). (out-Double)
typedef void (STDCALL *fnPtrGetInitialDrag)(double semiMajorAxis, double eccen, double* ndot, double* bstar);


// Converts covariance matrix PTW to UVW.
// PTW = P: TxW, T: along velocity direction, W: pos x vel.
// UVW = U: radial direction, V: in plane, perpendicular to U, W: pos x vel.
// pos                The input position vector (km). (in-Double[3])
// vel                The input velocity vector (km/s). (in-Double[3])
// ptwCovMtx          The PTW covariance matrix to be converted. (in-Double[6, 6])
// uvwCovMtx          The resulting UVW covariance matrix. (out-Double[6, 6])
typedef void (STDCALL *fnPtrCovMtxPTWToUVW)(double pos[3], double vel[3], double ptwCovMtx[6][6], double uvwCovMtx[6][6]);


// Converts covariance matrix UVW to PTW.
// PTW = P: TxW, T: along velocity direction, W: pos x vel.
// UVW = U: radial direction, V: in plane, perpendicular to U, W: pos x vel.
// pos                The input position vector (km). (in-Double[3])
// vel                The input velocity vector (km/s). (in-Double[3])
// uvwCovMtx          The UVW covariance matrix to be converted. (in-Double[6, 6])
// ptwCovMtx          The resulting PTW covariance matrix. (out-Double[6, 6])
typedef void (STDCALL *fnPtrCovMtxUVWToPTW)(double pos[3], double vel[3], double uvwCovMtx[6][6], double ptwCovMtx[6][6]);


// Computes Earth/Sensor/Earth Limb and Earth/Sensor/Satellite angles.
// earthLimb          Earth limb distance (km). (in-Double)
// satECI             Satellite position in ECI (km). (in-Double[3])
// senECI             Sensor position in ECI (km). (in-Double[3])
// earthSenLimb       The resulting earth/sensor/limb angle (deg). (out-Double)
// earthSenSat        The resulting earth/sensor/sat angle (deg). (out-Double)
// satEarthSen        The resulting sat/earth/sensor angle (deg). (out-Double)
typedef void (STDCALL *fnPtrEarthObstructionAngles)(double earthLimb, double satECI[3], double senECI[3], double* earthSenLimb, double* earthSenSat, double* satEarthSen);


// Determines if a point in space is sunlit at the input time ds50ET
// ds50ET             The number of days since 1950, ET for which to determine if the point is sunlit. (in-Double)
// ptEci              a position in ECI (km). (in-Double[3])
// returns 0=no, the specified point isn't sunlit, 1=yes, the specified point is sunlit
typedef int (STDCALL *fnPtrIsPointSunlit)(double ds50ET, double ptEci[3]);


// Rotates Right Ascension and Declination to specified epoch
// nutationTerms      Nutation terms (4-106, 4:least accurate, 106:most acurate). (in-Integer)
// dir                1: TEME of Date To MEME year of equinox, 2: MEME year of equinox to TEME of Date (in-Integer)
// ds50UTCIn          Origin time in days since 1950 UTC (in-Double)
// RAIn               Input right ascension (deg) (in-Double)
// declIn             Input declination (deg) (in-Double)
// ds50UTCOut         Destination time in days since 1950 UTC (in-Double)
// RAOut              Output right ascension (deg) (out-Double)
// declOut            Output declination (deg) (out-Double)
typedef void (STDCALL *fnPtrRotRADecl)(int nutationTerms, int dir, double ds50UTCIn, double RAIn, double declIn, double ds50UTCOut, double* RAOut, double* declOut);


// Rotates Right Ascension and Declination from TEME of Date to MEME of the specified year of equinox
// nutationTerms      Nutation terms (4-106, 4:least accurate, 106:most acurate). (in-Integer)
// yrOfEqnx           Year of equinox (=1: 0 Jan of input year, =2: J2000, =3: B1950) (in-Integer)
// ds50UTCIn          Input time in days since 1950 UTC (in-Double)
// RAIn               Input right ascension (deg) (in-Double)
// declIn             Input declination (deg) (in-Double)
// RAOut              Output right ascension (deg) (out-Double)
// declOut            Output declination (deg) (out-Double)
// returns 0 if the rotation was successful, non-0 if there is an error
typedef int (STDCALL *fnPtrRotRADec_DateToEqnx)(int nutationTerms, int yrOfEqnx, double ds50UTCIn, double RAIn, double declIn, double* RAOut, double* declOut);


// Rotates Right Ascension and Declination from MEME of the specified year of equinox to TEME of Date
// nutationTerms      Nutation terms (4-106, 4:least accurate, 106:most acurate). (in-Integer)
// yrOfEqnx           Year of equinox (=1: 0 Jan of input year, =2: J2000, =3: B1950) (in-Integer)
// ds50UTCIn          Input time in days since 1950 UTC (in-Double)
// RAIn               Input right ascension (deg) (in-Double)
// declIn             Input declination (deg) (in-Double)
// RAOut              Output right ascension (deg) (out-Double)
// declOut            Output declination (deg) (out-Double)
// returns 0 if the rotation was successful, non-0 if there is an error
typedef int (STDCALL *fnPtrRotRADec_EqnxToDate)(int nutationTerms, int yrOfEqnx, double ds50UTCIn, double RAIn, double declIn, double* RAOut, double* declOut);

// Index of Keplerian elements
static const int  
   XA_KEP_A     =   0,       // semi-major axis (km)
   XA_KEP_E     =   1,       // eccentricity (unitless)
   XA_KEP_INCLI =   2,       // inclination (deg)
   XA_KEP_MA    =   3,       // mean anomaly (deg)
   XA_KEP_NODE  =   4,       // right ascension of the asending node (deg)
   XA_KEP_OMEGA =   5,       // argument of perigee (deg)
   XA_KEP_SIZE  =  6;   
   
// Index of classical elements
static const int  
   XA_CLS_N     =   0,       // N mean motion (revs/day)
   XA_CLS_E     =   1,       // eccentricity (unitless)
   XA_CLS_INCLI =   2,       // inclination (deg)
   XA_CLS_MA    =   3,       // mean anomaly (deg)
   XA_CLS_NODE  =   4,       // right ascension of the asending node (deg)
   XA_CLS_OMEGA =   5,       // argument of perigee (deg)
   XA_CLS_SIZE  =   6;

// Index of equinoctial elements
static const int  
   XA_EQNX_AF   =   0,       // Af (unitless) 
   XA_EQNX_AG   =   1,       // Ag (unitless)
   XA_EQNX_CHI  =   2,       // Chi (unitless)
   XA_EQNX_PSI  =   3,       // Psi (unitless)
   XA_EQNX_L    =   4,       // L mean longitude (deg)
   XA_EQNX_N    =   5,       // N mean motion (revs/day)
   XA_EQNX_SIZE =   6;
   
// Indexes of AstroConvFrTo
static const int  
   XF_CONV_SGP42SGP = 101;        // SGP4 (A, E, Incli, BStar) to SGP (Ndot, N2Dot)


// Indexes for topocentric components
static const int  
   XA_TOPO_RA    = 0,         // Right ascension (deg)
   XA_TOPO_DEC   = 1,         // Declination (deg)
   XA_TOPO_AZ    = 2,         // Azimuth (deg)
   XA_TOPO_EL    = 3,         // Elevation (deg)
   XA_TOPO_RANGE = 4,         // Range (km)
   XA_TOPO_RADOT = 5,         // Right ascension dot (deg/s)
   XA_TOPO_DECDOT= 6,         // Declincation dot (deg/s)
   XA_TOPO_AZDOT = 7,         // Azimuth dot (deg/s)
   XA_TOPO_ELDOT = 8,         // Elevation dot (deg/s)
   XA_TOPO_RANGEDOT = 9,      // Range dot (km/s)   
   XA_TOPO_SIZE  = 10;   
   
   
// Indexes for RAE components
static const int  
   XA_RAE_RANGE   = 0,        // Range (km)
   XA_RAE_AZ      = 1,        // Azimuth (deg)
   XA_RAE_EL      = 2,        // Elevation (deg)
   XA_RAE_RANGEDOT= 3,        // Range dot (km/s)   
   XA_RAE_AZDOT   = 4,        // Azimuth dot (deg/s)
   XA_RAE_ELDOT   = 5,        // Elevation dot (deg/s)
   XA_RAE_SIZE    = 6;
   
   
// Year of Equinox indicator
static const int  
   YROFEQNX_OBTIME = 0,       // Date of observation
   YROFEQNX_CURR   = 1,       // 0 Jan of Date
   YROFEQNX_2000   = 2,       // J2000
   YROFEQNX_1950   = 3;       // B1950
   



// AstroFuncDll's function pointers
fnPtrAstroFuncInit                  AstroFuncInit;
fnPtrAstroFuncGetInfo               AstroFuncGetInfo;
fnPtrKepToEqnx                      KepToEqnx;
fnPtrKepToPosVel                    KepToPosVel;
fnPtrKepToUVW                       KepToUVW;
fnPtrClassToEqnx                    ClassToEqnx;
fnPtrEqnxToClass                    EqnxToClass;
fnPtrEqnxToKep                      EqnxToKep;
fnPtrEqnxToPosVel                   EqnxToPosVel;
fnPtrPosVelToEqnx                   PosVelToEqnx;
fnPtrPosVelMuToEqnx                 PosVelMuToEqnx;
fnPtrPosVelToKep                    PosVelToKep;
fnPtrPosVelMuToKep                  PosVelMuToKep;
fnPtrPosVelToUUVW                   PosVelToUUVW;
fnPtrPosVelToPTW                    PosVelToPTW;
fnPtrSolveKepEqtn                   SolveKepEqtn;
fnPtrCompTrueAnomaly                CompTrueAnomaly;
fnPtrNToA                           NToA;
fnPtrAToN                           AToN;
fnPtrKozaiToBrouwer                 KozaiToBrouwer;
fnPtrBrouwerToKozai                 BrouwerToKozai;
fnPtrKepOscToMean                   KepOscToMean;
fnPtrXYZToLLH                       XYZToLLH;
fnPtrXYZToLLHTime                   XYZToLLHTime;
fnPtrLLHToXYZ                       LLHToXYZ;
fnPtrLLHToXYZTime                   LLHToXYZTime;
fnPtrEFGToECI                       EFGToECI;
fnPtrEFGToECITime                   EFGToECITime;
fnPtrECIToEFG                       ECIToEFG;
fnPtrECIToEFGTime                   ECIToEFGTime;
fnPtrECRToEFG                       ECRToEFG;
fnPtrECRToEFGTime                   ECRToEFGTime;
fnPtrEFGToECR                       EFGToECR;
fnPtrEFGToECRTime                   EFGToECRTime;
fnPtrEFGPosToLLH                    EFGPosToLLH;
fnPtrLLHToEFGPos                    LLHToEFGPos;
fnPtrRotJ2KToDate                   RotJ2KToDate;
fnPtrRotDateToJ2K                   RotDateToJ2K;
fnPtrCompSunMoonPos                 CompSunMoonPos;
fnPtrCompSunPos                     CompSunPos;
fnPtrCompMoonPos                    CompMoonPos;
fnPtrAstroConvFrTo                  AstroConvFrTo;
fnPtrRADecToLAD                     RADecToLAD;
fnPtrAzElToLAD                      AzElToLAD;
fnPtrECIToTopoComps                 ECIToTopoComps;
fnPtrRaDecToAzEl                    RaDecToAzEl;
fnPtrRaDecToAzElTime                RaDecToAzElTime;
fnPtrAzElToRaDec                    AzElToRaDec;
fnPtrAzElToRaDecTime                AzElToRaDecTime;
fnPtrRAEToECI                       RAEToECI;
fnPtrGetInitialDrag                 GetInitialDrag;
fnPtrCovMtxPTWToUVW                 CovMtxPTWToUVW;
fnPtrCovMtxUVWToPTW                 CovMtxUVWToPTW;
fnPtrEarthObstructionAngles         EarthObstructionAngles;
fnPtrIsPointSunlit                  IsPointSunlit;
fnPtrRotRADecl                      RotRADecl;
fnPtrRotRADec_DateToEqnx            RotRADec_DateToEqnx;
fnPtrRotRADec_EqnxToDate            RotRADec_EqnxToDate;



void LoadAstroFuncDll(const char* libPath);
void FreeAstroFuncDll();




#endif
// ========================= End of auto generated code ==========================
