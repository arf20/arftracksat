// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#ifndef SGP4PROPDLL_H
#define SGP4PROPDLL_H

#include "../services/DllUtils.h"

// Provide the path to the dll/so
#ifdef _WIN32
  #define Sgp4PropDll "Sgp4Prop.dll"
#else
  #define Sgp4PropDll "libsgp4prop.so"
#endif


// Initializes the Sgp4 DLL for use in the program.
// 
// If this function returns an error, it is recommended that you stop the program immediately.
// 
// An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
// apPtr              The handle that was returned from DllMainInit(). See the documentation for DllMain.dll for details. (in-Long)
// returns 0 if Sgp4Prop.dll is initialized successfully, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4Init)(__int64 apPtr);


// Returns information about the current version of Sgp4Prop.dll. The information is placed in the string parameter you pass in.
// The returned string provides information about the version number, build date, and platform.
// infoStr            A string to hold the information about Sgp4Prop.dll. (out-Character[128])
typedef void (STDCALL *fnPtrSgp4GetInfo)(char infoStr[128]);


// Initializes an SGP4 satellite from an SGP or SGP4 TLE.
// Internally, when this function is called, Tle.dll's set of TLEs is searched for the provided satKey. If found, the associated TLE data will be used to create an SGP4 satellite which then will be added to Sgp4Prop.dll's set of satellites. Subsequent calls to propagate this satellite will use the data in this set to compute the satellite's new state.
// 
// This routine should be called once for each satellite you wish to propagate before propagation begins, or any time the associated data that is stored by Tle.dll is changed.
// 
// The call to this routine needs to be placed before any calls to the SGP4 propagator routines (Sgp4PropMse(), Sgp4PropDs50UTC(), etc.).
// satKey             The satellite's unique key. This key will have been returned by one of the routines in Tle.dll. (in-Long)
// returns 0 if the satellite is successfully initialized and added to Sgp4Prop.dll's set of satellites, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4InitSat)(__int64 satKey);


// Removes a satellite, represented by the satKey, from the set of satellites.
// If the specified satKey is not currently loaded into the propagator, an error will be indicated.
// 
// Removing a satellite from the propagator's set of satellites does not affect the corresponding TLE data loaded by calls to routines in Tle.dll.
// satKey             The satellite's unique key. (in-Long)
// returns 0 if the satellite is removed successfully, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4RemoveSat)(__int64 satKey);


// Removes all currently loaded satellites from memory.
// Calling this function removes all satellites from the set maintained by Sgp4Prop.dll. However, the TLE data loaded by Tle.dll is unaffected by this function.
// returns 0 if all satellites are removed successfully from memory, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4RemoveAllSats)();


// Returns the number of GP objects currently created. 
// returns The number of GP objects currently created.
typedef int (STDCALL *fnPtrSgp4GetCount)();


// Propagates a satellite, represented by the satKey, to the time expressed in minutes since the satellite's epoch time. 
// The resulting data about the satellite is placed in the various reference parameters.
// It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
// 
// This function can be called in random time requests.
// The following cases will result in an error:
// 
// Semi major axis A 1.0D6
// Eccentricity E >= 1.0 or E 
// Mean anomaly MA>=1.0D10
// Hyperbolic orbit E2>= 1.0
// satKey doesn't exist in the set of loaded satellites
// FK model not set to FK5
// 
// satKey             The satellite's unique key. (in-Long)
// mse                The time to propagate to, specified in minutes since the satellite's epoch time. (in-Double)
// ds50UTC            Resulting time in days since 1950, UTC. (out-Double)
// pos                Resulting ECI position vector (km) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// vel                Resulting ECI velocity vector (km/s) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// llh                Resulting geodetic latitude (deg), longitude(deg), and height (km). (out-Double[3])
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4PropMse)(__int64 satKey, double mse, double* ds50UTC, double pos[3], double vel[3], double llh[3]);


// Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
// The resulting data about the satellite is placed in the pos (position), vel (velocity), and llh (Lat/Lon/Height) parameters.
// It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
// The following cases will result in an error:
// 
// Semi major axis A 1.0D6
// Eccentricity E >= 1.0 or E 
// Mean anomaly MA>=1.0D10
// Hyperbolic orbit E2>= 1.0
// satKey doesn't exist in the set of loaded satellites
// GEO model not set to WGS-72 and/or FK model not set to FK5
// 
// satKey             The unique key of the satellite to propagate. (in-Long)
// ds50UTC            The time to propagate to, expressed in days since 1950, UTC. (in-Double)
// mse                Resulting time in minutes since the satellite's epoch time. (out-Double)
// pos                Resulting ECI position vector (km) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// vel                Resulting ECI velocity vector (km/s) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// llh                Resulting geodetic latitude (deg), longitude(deg), and height (km). (out-Double[3])
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4PropDs50UTC)(__int64 satKey, double ds50UTC, double* mse, double pos[3], double vel[3], double llh[3]);


// Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
// The resulting data about the satellite is placed in the pos (position), vel (velocity) parameters.
// satKey             The unique key of the satellite to propagate. (in-Long)
// ds50UTC            The time to propagate to, expressed in days since 1950, UTC. (in-Double)
// pos                Resulting ECI position vector (km) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// vel                Resulting ECI velocity vector (km/s) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4PropDs50UtcPosVel)(__int64 satKey, double ds50UTC, double pos[3], double vel[3]);


// Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
// Only the geodetic information is returned by this function.
// It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
// 
// This function is similar to Sgp4PropDs50UTC but returns only LLH.  This function is designed especially for applications which plot ground traces.
// The following cases will result in an error:
// 
// Semi major axis A 1.0D6
// Eccentricity E >= 1.0 or E 
// Mean anomaly MA>=1.0D10
// Hyperbolic orbit E2>= 1.0
// satKey doesn't exist in the set of loaded satellites
// GEO model not set to WGS-72 and/or FK model not set to FK5
// 
// satKey             The unique key of the satellite to propagate. (in-Long)
// ds50UTC            The time to propagate to, expressed in days since 1950, UTC. (in-Double)
// llh                Resulting geodetic latitude (deg), longitude(deg), and height (km). (out-Double[3])
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4PropDs50UtcLLH)(__int64 satKey, double ds50UTC, double llh[3]);


// Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
// Only the ECI position vector is returned by this function.
// It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
// 
// This function is similar to Sgp4PropDs50UTC but returns only ECI position vector.  This function is designed especially for applications which plot satellite position in 3D.
// The following cases will result in an error:
// 
// Semi major axis A 1.0D6
// Eccentricity E >= 1.0 or E 
// Mean anomaly MA>=1.0D10
// Hyperbolic orbit E2>= 1.0
// satKey doesn't exist in the set of loaded satellites
// GEO model not set to WGS-72 and/or FK model not set to FK5
// 
// satKey             The unique key of the satellite to propagate. (in-Long)
// ds50UTC            The time to propagate to, expressed in days since 1950, UTC. (in-Double)
// pos                Resulting ECI position vector (km) in True Equator and Mean Equinox of Epoch. (out-Double[3])
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4PropDs50UtcPos)(__int64 satKey, double ds50UTC, double pos[3]);


// Retrieves propagator's precomputed results. This function can be used to obtain results from 
// a propagation which are not made available through calls to the propagation functions themselves.
// 
// See example in Sgp4PropMse or Sgp4PropDs50UTC.
// 
// This function should be called immediately after a successful call to Sgp4PropMse() or Sgp4PropDs50UTC() to retrieve the desired values.
// 
// It is the caller's responsibility to ensure that the array passed in the destArray parameter is large enough to hold the requested values. The required size can be found by looking at the destArray size column of the table below describing valid index values.
// 
// The destArray Arrangement column lists the order of the elements in the array. It is not necessarily the subscript of the element in the array since this is language-dependent. For example, in C/C++ the first element in every array is the zero-subscripted element. In other programming languages, the subscript of the first element is 1.
// 
// Note: This function is not thread safe, please use Sgp4PropAll() instead 
// 
// The table below shows the values for the xf_Sgp4Out parameter:
// 
// 
// Index
// Index Interpretation
// DestArray size
// DestArray Arrangement
// 
// 1Revolution number1Revolution number (based on the Osculating Keplerian
// Elements)
// 2Nodal Apogee Perigee3nodal period (minutes)apogee
// (km)perigee (km)
// 3Mean Keplerian Elements6semi-major axis (km)eccentricity
// (unitless)inclination (degree)mean anomaly (degree)right ascension of the ascending node
// (degree)argument of perigee (degree)
// 4Osculating Keplerian Elements6Same as Mean Keplerian Elements
// 
// satKey             The unique key of the satellite for which to retrieve results. (in-Long)
// xf_Sgp4Out         Specifies which propagator outputs to retrieve. (in-Integer)
// destArr            Array to receive the resulting propagator outputs. (out-Double[*])
// returns 0 if the requested information is retrieved successfully, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4GetPropOut)(__int64 satKey, int xf_Sgp4Out, double destArr[]);


// Propagates a satellite, represented by the satKey, to the time expressed in either minutes since epoch or days since 1950, UTC. 
// All propagation data is returned by this function.
// satKey             The unique key of the satellite to propagate. (in-Long)
// timeType           The propagation time type: 0 = minutes since epoch, 1 = days since 1950, UTC (in-Integer)
// timeIn             The time to propagate to, expressed in either minutes since epoch or days since 1950, UTC. (in-Double)
// xa_Sgp4Out         The array that stores all Sgp4 propagation data, see XA_SGP4OUT_? for array arrangement (out-Double[64])
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4PropAll)(__int64 satKey, int timeType, double timeIn, double xa_Sgp4Out[64]);


// Converts osculating position and velocity vectors to a set of mean Keplerian SGP4 elements.
// The new position and velocity vectors are the results of using SGP4 propagator to propagate the computed sgp4MeanKep to the time specified in year and day of epoch time. They should be closely matched with the input osculating position and velocity vectors.
// 
// The mean Keplerian elements are SGP4's Brouwer mean motion not SGP's Kozai mean motion.
// yr                 2 or 4 digit year of the epoch time. (in-Integer)
// day                Day of year of the epoch time. (in-Double)
// pos                Input osculating position vector (km). (in-Double[3])
// vel                Input osculating velocity vector (km/s). (in-Double[3])
// posNew             Resulting position vector (km) propagated from the sgp4MeanKep. (out-Double[3])
// velNew             Resulting velocity vector (km/s) propagated from the sgp4MeanKep. (out-Double[3])
// sgp4MeanKep        Resulting set of Sgp4 mean Keplerian elements. (out-Double[6])
// returns 0 if the conversion is successful, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4PosVelToKep)(int yr, double day, double pos[3], double vel[3], double posNew[3], double velNew[3], double sgp4MeanKep[6]);


// Converts osculating position and velocity vectors to TLE array - allows bstar/bterm, drag values to be used in the conversion if desired
// The function is similar to Sgp4PosVelToKep but allows the user to specify agom (XP mode) and bstar/bterm values, if desired, to be used in solving for the new Keplerian elements.
// 
// The updated elements returned in the xa_tle array is of type SGP and the mean motion is Kozai mean motion.
// pos                Input osculating position vector (km). (in-Double[3])
// vel                Input osculating velocity vector (km/s). (in-Double[3])
// xa_tle             Input/Output array containing TLE's numerical fields, see XA_TLE_? for array arrangement; required data include epoch, ephemType, drag, bstar/bterm (out-Double[64])
// returns 0 if the conversion is successful, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4PosVelToTleArr)(double pos[3], double vel[3], double xa_tle[64]);


// Reepochs a loaded TLE, represented by the satKey, to a new epoch.
// satKey             The unique key of the satellite to reepoch. (in-Long)
// reepochDs50UTC     The new epoch, express in days since 1950, UTC. (in-Double)
// line1Out           A string to hold the first line of the reepoched TLE. (out-Character[512])
// line2Out           A string to hold the second line of the reepoched TLE. (out-Character[512])
// returns 0 if the reepoch is successful, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4ReepochTLE)(__int64 satKey, double reepochDs50UTC, char line1Out[512], char line2Out[512]);


// Reepochs a loaded TLE, represented by the satKey, to a new epoch in Csv format.
// satKey             The unique key of the satellite to reepoch. (in-Long)
// reepochDs50UTC     The new epoch, express in days since 1950, UTC. (in-Double)
// csvLine            A string to hold the reepoched CSV. (out-Character[512])
// returns 0 if the reepoch is successful, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4ReepochCsv)(__int64 satKey, double reepochDs50UTC, char csvLine[512]);


// Sets path to the Sgp4 Open License file if the license file is not in the current working folder
// Note: Remember to call this function even before calling the Sgp4Init.
// The licFilePath can contain either an absolute or relative path to the 
// folder containing the license file, but it must include a trailing 
// path separator.  I.e. "./license/" not "./license"
// licFilePath        The file path to the Sgp4 Open License file (in-Character[512])
typedef void (STDCALL *fnPtrSgp4SetLicFilePath)(char licFilePath[512]);


// Gets the current path to the Sgp4 Open License file
// licFilePath        The file path to the Sgp4 Open License file (out-Character[512])
typedef void (STDCALL *fnPtrSgp4GetLicFilePath)(char licFilePath[512]);


// Generates ephemerides for the input satellite, represented by its satKey, for the specified time span and step size
// Notes: if arrSize isn't big enough to store all the ephemeris points, the function will exit when the ephemArr reaches
// that many points (arrSize) and the errCode is set to IDX_ERR_WARN
// satKey             The unique key of the satellite to generate ephemerides. (in-Long)
// startTime          Start time expressed in days since 1950, UTC. (in-Double)
// endTime            End time expressed in days since 1950, UTC. (in-Double)
// stepSize           Step size in minutes (static); enter predefine negative values (DYN_SS_xxx) to request dynamic step size (in-Double)
// sgp4_ephem         Output ephemeris type 1=ECI, 2=J2K. (in-Integer)
// arrSize            Size of input ephemArr (in-Integer)
// ephemArr           Output ephemerides - 0: time in days since 1950 UTC, 1-3: pos (km), 4-6: vel (km/sec) (out-Double[*, 7])
// genEphemPts        Actual number of ephemeris points generated (always &le; arrSize) (out-Integer)
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4GenEphems)(__int64 satKey, double startTime, double endTime, double stepSize, int sgp4_ephem, int arrSize, double ephemArr[][7], int* genEphemPts);


// Generates ephemerides for the input TLE - in an array format - for the specified time span and step size (IOC - In Once Call)
// Notes: 
// - This function takes in TLE data directly and doesn't need to go through loading/geting satKey/initializing steps
// - if arrSize isn't big enough to store all the ephemeris points, the function will exit when the ephemArr reaches
//   that many points (arrSize) and the errCode is set to IDX_ERR_WARN
// xa_tle             Input array containing TLE's numerical fields, see XA_TLE_? for array arrangement (in-Double[64])
// startTime          Start time expressed in days since 1950, UTC. (in-Double)
// endTime            End time expressed in days since 1950, UTC. (in-Double)
// stepSize           Step size in minutes (static); enter predefine negative values (DYN_SS_xxx) to request dynamic step size (in-Double)
// sgp4_ephem         Output ephemeris type 1=ECI, 2=J2K. (in-Integer)
// arrSize            Size of input ephemArr (in-Integer)
// ephemArr           Output ephemerides - 0: time in days since 1950 UTC, 1-3: pos (km), 4-6: vel (km/sec) (out-Double[*, 7])
// genEphemPts        Actual number of ephemeris points generated (always &le; arrSize) (out-Integer)
// returns 0 if the propagation is successful, non-0 if there is an error (see error decoder in GP_ERR_xxx).
typedef int (STDCALL *fnPtrSgp4GenEphems_OS)(double xa_tle[64], double startTime, double endTime, double stepSize, int sgp4_ephem, int arrSize, double ephemArr[][7], int* genEphemPts);


// Propagates all input satellites, represented by their satKeys, to the time expressed in days since 1950, UTC. 
// satKeys            The satellite keys of all input satellites (in-Long[*])
// numOfSats          The total number of satellites (in-Integer)
// ds50UTC            The time to propagate all satelllites to, expressed in days since 1950, UTC. (in-Double)
// ephemArr           0-2: pos (km), 3-5: vel (km/sec) (out-Double[*, 6])
// returns 0 if the propagation is successful, non-0 if there is an error.
typedef int (STDCALL *fnPtrSgp4PropAllSats)(__int64 satKeys[], int numOfSats, double ds50UTC, double ephemArr[][6]);

// Different time types for passing to Sgp4PropAll
static const int  
   SGP4_TIMETYPE_MSE      = 0,   // propagation time is in minutes since epoch
   SGP4_TIMETYPE_DS50UTC  = 1;   // propagation time is in days since 1950, UTC
   
// Sgp4 propagated output fields
static const int  
   XF_SGP4OUT_REVNUM       = 1,    // Revolution number
   XF_SGP4OUT_NODAL_AP_PER = 2,    // Nodal period, apogee, perigee
   XF_SGP4OUT_MEAN_KEP     = 3,    // Mean Keplerian
   XF_SGP4OUT_OSC_KEP      = 4;    // Osculating Keplerian
   
// Sgp4 propagated data
static const int  
   XA_SGP4OUT_DS50UTC      =  0,   // Propagation time in days since 1950, UTC
   XA_SGP4OUT_MSE          =  1,   // Propagation time in minutes since the satellite's epoch time
   XA_SGP4OUT_POSX         =  2,   // ECI X position (km) in True Equator and Mean Equinox of Epoch
   XA_SGP4OUT_POSY         =  3,   // ECI Y position (km) in True Equator and Mean Equinox of Epoch
   XA_SGP4OUT_POSZ         =  4,   // ECI Z position (km) in True Equator and Mean Equinox of Epoch
   XA_SGP4OUT_VELX         =  5,   // ECI X velocity (km/s) in True Equator and Mean Equinox of Epoch
   XA_SGP4OUT_VELY         =  6,   // ECI Y velocity (km/s) in True Equator and Mean Equinox of Epoch
   XA_SGP4OUT_VELZ         =  7,   // ECI Z velocity (km/s) in True Equator and Mean Equinox of Epoch
   XA_SGP4OUT_LAT          =  8,   // Geodetic latitude (deg)
   XA_SGP4OUT_LON          =  9,   // Geodetic longitude (deg)
   XA_SGP4OUT_HEIGHT       = 10,   // Height above geoid (km)
   XA_SGP4OUT_REVNUM       = 11,   // Revolution number
   XA_SGP4OUT_NODALPER     = 12,   // Nodal period (min)
   XA_SGP4OUT_APOGEE       = 13,   // Apogee (km)
   XA_SGP4OUT_PERIGEE      = 14,   // Perigee (km)
   XA_SGP4OUT_MN_A         = 15,   // Mean semi-major axis (km)
   XA_SGP4OUT_MN_E         = 16,   // Mean eccentricity (unitless)
   XA_SGP4OUT_MN_INCLI     = 17,   // Mean inclination (deg)
   XA_SGP4OUT_MN_MA        = 18,   // Mean mean anomaly (deg)
   XA_SGP4OUT_MN_NODE      = 19,   // Mean right ascension of the asending node (deg)
   XA_SGP4OUT_MN_OMEGA     = 20,   // Mean argument of perigee (deg)
   XA_SGP4OUT_OSC_A        = 21,   // Osculating semi-major axis (km)  
   XA_SGP4OUT_OSC_E        = 22,   // Osculating eccentricity (unitless)
   XA_SGP4OUT_OSC_INCLI    = 23,   // Osculating inclination (deg)
   XA_SGP4OUT_OSC_MA       = 24,   // Osculating mean anomaly (deg)
   XA_SGP4OUT_OSC_NODE     = 25,   // Osculating right ascension of the asending node (deg)
   XA_SGP4OUT_OSC_OMEGA    = 26,   // Osculating argument of perigee (deg)

   XA_SGP4OUT_SIZE         = 64;
   
// Different options for generating ephemerides from SGP4
static const int  
   SGP4_EPHEM_ECI   = 1,       // ECI TEME of DATE     - 0: time in days since 1950 UTC, 1-3: pos (km), 4-6: vel (km/sec)
   SGP4_EPHEM_J2K   = 2;       // MEME of J2K (4 terms)- 0: time in days since 1950 UTC, 1-3: pos (km), 4-6: vel (km/sec) 
   

// Different dynamic step size options
static const int     
   DYN_SS_BASIC  = -1   ; // Use a simple algorithm to determine step size based on satellite's current position
   
//*******************************************************************************

   
// Different return values of errCode from Sgp4 propagation
static const int  
   GP_ERR_NONE        = 0,    // SGP4 propagates successfully
   GP_ERR_BADFK       = 1,    // Bad FK model (FK5 must be selected)
   GP_ERR_ANEGATIVE   = 2,    // A is negative
   GP_ERR_ATOOLARGE   = 3,    // A is to large
   GP_ERR_EHYPERPOLIC = 4,    // Eccentricity is hyperbolic
   GP_ERR_ENEGATIVE   = 5,    // Eccentricity is negative
   GP_ERR_MATOOLARGE  = 6,    // Mean anomaly is too large
   GP_ERR_E2TOOLARGE  = 7;    // e**2 is too large


//*******************************************************************************

   



// Sgp4PropDll's function pointers
fnPtrSgp4Init                       Sgp4Init;
fnPtrSgp4GetInfo                    Sgp4GetInfo;
fnPtrSgp4InitSat                    Sgp4InitSat;
fnPtrSgp4RemoveSat                  Sgp4RemoveSat;
fnPtrSgp4RemoveAllSats              Sgp4RemoveAllSats;
fnPtrSgp4GetCount                   Sgp4GetCount;
fnPtrSgp4PropMse                    Sgp4PropMse;
fnPtrSgp4PropDs50UTC                Sgp4PropDs50UTC;
fnPtrSgp4PropDs50UtcPosVel          Sgp4PropDs50UtcPosVel;
fnPtrSgp4PropDs50UtcLLH             Sgp4PropDs50UtcLLH;
fnPtrSgp4PropDs50UtcPos             Sgp4PropDs50UtcPos;
fnPtrSgp4GetPropOut                 Sgp4GetPropOut;
fnPtrSgp4PropAll                    Sgp4PropAll;
fnPtrSgp4PosVelToKep                Sgp4PosVelToKep;
fnPtrSgp4PosVelToTleArr             Sgp4PosVelToTleArr;
fnPtrSgp4ReepochTLE                 Sgp4ReepochTLE;
fnPtrSgp4ReepochCsv                 Sgp4ReepochCsv;
fnPtrSgp4SetLicFilePath             Sgp4SetLicFilePath;
fnPtrSgp4GetLicFilePath             Sgp4GetLicFilePath;
fnPtrSgp4GenEphems                  Sgp4GenEphems;
fnPtrSgp4GenEphems_OS               Sgp4GenEphems_OS;
fnPtrSgp4PropAllSats                Sgp4PropAllSats;



void LoadSgp4PropDll(const char* libPath);
void FreeSgp4PropDll();




#endif
// ========================= End of auto generated code ==========================
