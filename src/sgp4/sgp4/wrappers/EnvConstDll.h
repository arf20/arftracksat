// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#ifndef ENVCONSTDLL_H
#define ENVCONSTDLL_H

#include "../services/DllUtils.h"

// Provide the path to the dll/so
#ifdef _WIN32
  #define EnvConstDll "EnvConst.dll"
#else
  #define EnvConstDll "libenvconst.so"
#endif


// Initializes the EnvInit DLL for use in the program.
// If this function returns an error, it is recommended that you stop the program immediately.
// 
// An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
// 
// When the function is called, the GEO model is set to WGS-72 and the FK model is set to FK5.  If the user plans to use the SGP4 propagator, DO NOT change this default setting. Otherwise, SGP4 won't work
// apPtr              The handle that was returned from DllMainInit, see the documentation for DllMain.dll for details. (in-Long)
// returns Returns zero indicating the EnvConst DLL has been initialized successfully. Other values indicate an error.
typedef int (STDCALL *fnPtrEnvInit)(__int64 apPtr);


// Returns information about the EnvConst DLL.
// The returned string provides information about the version number, build date, and the platform of the EnvConst DLL.
// infoStr            A string to hold the information about EnvConst.dll. (out-Character[128])
typedef void (STDCALL *fnPtrEnvGetInfo)(char infoStr[128]);


// Reads Earth constants (GEO) model and fundamental catalogue (FK) model settings from a file.
// The users can use NAME=VALUE pair to setup the GEO and FK models in the input file. 
// 
// For GEO model, the valid names are GEOCONST, BCONST and the valid values are WGS-72, WGS72,  72, WGS-84, WGS84, 84, EGM-96, EGM96, 96, EGM-08, EGM08, 08, JGM-2, JGM2, 2, SEM68R, 68, GEM5, 5, GEM9, and 9.
// 
// For FK model, the valid name is FKCONST and the valid values are: FK4, 4, FK5, 5.
// 
// All the string literals are case-insensitive.	
// envFile            The name of the input file. (in-Character[512])
// returns Returns zero indicating the input file has been loaded successfully. Other values indicate an error.
typedef int (STDCALL *fnPtrEnvLoadFile)(char envFile[512]);


// Saves the current Earth constants (GEO) model and fundamental catalogue (FK) model settings to a file.
// Returns zero indicating the GEO and FK settings have been successfully saved to the file. Other values indicate an error.
// envConstFile       The name of the file in which to save the settings. (in-Character[512])
// saveMode           Specifies whether to create a new file or append to an existing one. (0 = create, 1= append) (in-Integer)
// saveForm           Specifies the mode in which to save the file. (0 = text format, 1 = xml (not yet implemented, reserved for future)) (in-Integer)
// returns Returns zero indicating the GEO and FK settings have been successfully saved to the file. Other values indicate an error.
typedef int (STDCALL *fnPtrEnvSaveFile)(char envConstFile[512], int saveMode, int saveForm);


// Returns the current fundamental catalogue (FK) setting. 
// The FK model is shared among all the Standardized Astrodynamic Algorithms DLLs in the program. 
// returns Return the current FK setting as an integer. Valid values are: (4 = FK4, 5 = FK5)
typedef int (STDCALL *fnPtrEnvGetFkIdx)();


// Changes the fundamental catalogue (FK) setting to the specified value. 
// If the users enter an invalid value for the fkIdx, the program will continue to use the current setting. 
// 
// The FK model is globally shared among the Standardized Astrodynamic Algorithms DLLs. If its setting is changed, the new setting takes effect immediately.
// The FK model must be set to FK5 to use the SGP4 propagator.
// xf_FkMod           Specifies the FK model to use. The following values are accepted: xf_FkMod= 4: FK4, xf_FkMod= 5: FK5 (in-Integer)
typedef void (STDCALL *fnPtrEnvSetFkIdx)(int xf_FkMod);


// Returns the current Earth constants (GEO) setting. 
// 
// The GEO model is shared among all the Standardized Astrodynamic Algorithms DLLs in the program.
// 
// The following table lists possible values of the return value GEO setting:
// 
// 
// Value
// Value interpretation
// 
// 84WGS-84
// 96EGM-96
// 08EGM-08
// 72WGS-72 (default)
// 2JGM2
// 68STEM68R, SEM68R
// 5GEM5
// 9GEM9
// 
// returns The current GEO setting, expressed as an integer.
typedef int (STDCALL *fnPtrEnvGetGeoIdx)();


// Changes the Earth constants (GEO) setting to the specified value.
// 
// If you specify an invalid value for xf_GeoMod, the program will continue to use the current setting. 
// 
// The GEO model is globally shared among the Standardized Astrodynamic Algorithms DLLs. If its setting is changed, the new setting takes effect immediately
// 
// The following table lists possible values of the parameter value GEO setting:
// 
// 
// Value
// Value interpretation
// 
// 84WGS-84
// 96EGM-96
// 08EGM-08
// 72WGS-72 (default)
// 2JGM2
// 68STEM68R, SEM68R
// 5GEM5
// 9GEM9
// 
// 
// The GEO model must be set to WGS-72 to use the SGP4 propagator.
// xf_GeoMod          Specifies the GEO model to use. (in-Integer)
typedef void (STDCALL *fnPtrEnvSetGeoIdx)(int xf_GeoMod);


// Returns the name of the current Earth constants (GEO) model. 
// 
// The geoStr parameter may contain one of the following values:
// 
// WGS-84
// EGM-96
// EGM-08
// WGS-72
// JGM2
// SEM68R
// GEM5
// GEM9
// 
// geoStr             A string to store the name of the current GEO model. (out-Character[6])
typedef void (STDCALL *fnPtrEnvGetGeoStr)(char geoStr[6]);


// Changes the Earth constants (GEO) setting to the model specified by a string literal. 
// 
// If you specify an invalid value for geoStr, the program will continue to use the current setting.
// 
// The GEO model is globally shared among the Standardized Astrodynamic Algorithms DLLs. If its setting is changed, the new setting takes effect immediately.
// 
// The following table lists possible values of the parameter value GEO setting:
// 
// 
// geoStr (any string in the row)
// Interpretation
// 
// 'WGS-84', 'WGS84', '84'WGS-84
// 'EGM-96', 'EGM96', '96'EGM-96
// 'EGM-08', 'EGM08', '8'EGM-08
// 'WGS-72', 'WGS72', '72'WGS-72 (default)
// 'JGM-2, 'JGM2', '2'JGM-2
// 'SEM68R', '68'STEM68R, SEM68R
// 'GEM5', '5'GEM5
// 'GEM9', '9'GEM9
//    
// 
// The GEO model must be set to WGS-72 to use the SGP4 propagator.
// geoStr             The GEO model to use, expressed as a string. (in-Character[6])
typedef void (STDCALL *fnPtrEnvSetGeoStr)(char geoStr[6]);


// Retrieves the value of one of the constants from the current Earth constants (GEO) model. 
// 
// The following table lists possible values of the parameter value xf_GeoCon:
// 
// 
// xf_GeoCon
// xf_GeoCon Interpretation
// 
// 1FF: Earth flattening (reciprocal; unitless)
// 2J2 (unitless)
// 3J3 (unitless)
// 4J4 (unitless)
// 5KE (er^1.5/min)
// 6KMPER: Earth radius (km/er)
// 7RPTIM: Earth rotation rate w.r.t. fixed equinox (rad/min)
// 8CK2: J2/2 (unitless)
// 9CK4: -3/8 J4 (unitless)
// 10KS2EK: Converts km/sec to er/kem
// 11THDOT: Earth rotation rate w.r.t. fixed equinox (rad/kem)
//    
// xf_GeoCon          An index specifying the constant you wish to retrieve, see XF_GEOCON_? for field specification (in-Integer)
// returns Value of the requested GEO constant
typedef double (STDCALL *fnPtrEnvGetGeoConst)(int xf_GeoCon);


// Retrieves the value of one of the constants from the current fundamental catalogue (FK) model.
// 
// The following table lists possible values of the parameter value xf_FkCon:
// 
// 
// xf_FkCon
// xf_FkCon Interpretation
// 
// 1C1: Earth rotation rate w.r.t. moving equinox (rad/day)
// 2C1DOT: Earth rotation acceleration (rad/day2)
// 3THGR70: Greenwich angle (1970; rad)
//    
// xf_FkCon           An index specifying the constant you wish to retrieve, , see XF_FKCON_? for field specification (in-Integer)
// returns Value of the requested FK constant
typedef double (STDCALL *fnPtrEnvGetFkConst)(int xf_FkCon);


// Returns a handle that can be used to access the fundamental catalogue (FK) data structure. 
// 
// This function is needed when calling the ThetaGrnwch function from TimeFunc.dll.
// 
// The handle returned by this function is sometimes called a pointer for historical reasons. The name EnvGetFkPtr comes from the fact that the handle used to be called a pointer.
// returns A handle which can be used to access the FK data structure.
typedef __int64 (STDCALL *fnPtrEnvGetFkPtr)();


// Specifies the shape of the earth that will be used by the Astro Standards software, either spherical earth or oblate earth
// earthShape         The value indicates the shape of the earth: 0=spherical earth, 1= oblate earth (default) (in-Integer)
typedef void (STDCALL *fnPtrEnvSetEarthShape)(int earthShape);


// Returns the value representing the shape of the earth being used by the Astro Standards software, either spherical earth or oblate earth
// returns The value indicates the shape of the earth that is being used in the Astro Standards software: 0=spherical earth, 1= oblate earth
typedef int (STDCALL *fnPtrEnvGetEarthShape)();

// Indexes of Earth Constant fields
static const int     
   XF_GEOCON_FF    = 1,         // Earth flattening (reciprocal; unitless)
   XF_GEOCON_J2    = 2,         // J2 (unitless)
   XF_GEOCON_J3    = 3,         // J3 (unitless)
   XF_GEOCON_J4    = 4,         // J4 (unitless)
   XF_GEOCON_KE    = 5,         // Ke (er**1.5/min)
   XF_GEOCON_KMPER = 6,         // Earth radius (km/er)
   XF_GEOCON_RPTIM = 7,         // Earth rotation rate w.r.t. fixed equinox (rad/min)

   XF_GEOCON_CK2   = 8,         // J2/2 (unitless)
   XF_GEOCON_CK4   = 9,         // -3/8 J4 (unitless)
   XF_GEOCON_KS2EK = 10,        // Converts km/sec to er/kem
   XF_GEOCON_THDOT = 11,        // Earth rotation rate w.r.t. fixed equinox (rad/kemin)
   XF_GEOCON_J5    = 12;        // J5 (unitless)
   

// Indexes of FK Constant fields
static const int                     
   XF_FKCON_C1     = 1,         // Earth rotation rate w.r.t. moving equinox (rad/day) 
   XF_FKCON_C1DOT  = 2,         // Earth rotation acceleration(rad/day**2) 
   XF_FKCON_THGR70 = 3;         // Greenwich angle (1970; rad) 

// Indexes represent geopotential models GEO
static const int  
   XF_GEOMOD_WGS84  =   84,     // Earth constants - WGS-84
   XF_GEOMOD_EGM96  =   96,     // Earth constants - EGM-96
   XF_GEOMOD_EGM08  =    8,     // Earth constants - EGM-08
   XF_GEOMOD_WGS72  =   72,     // Earth constants - WGS-72
   XF_GEOMOD_JGM2   =    2,     // Earth constants - JGM2
   XF_GEOMOD_STEM68 =   68,     // Earth constants - STEM68
   XF_GEOMOD_GEM5   =    5,     // Earth constants - GEM5
   XF_GEOMOD_GEM9   =    9,     // Earth constants - GEM9
   XF_GEOMOD_UNKNOWN=  100;

//*******************************************************************************

// Indexes represent fundamental catalogue FK
static const int  
   XF_FKMOD_4 = 4,    // Fundamental Catalog - FK5
   XF_FKMOD_5 = 5;    // Fundamental Catalog - FK4





// EnvConstDll's function pointers
fnPtrEnvInit                        EnvInit;
fnPtrEnvGetInfo                     EnvGetInfo;
fnPtrEnvLoadFile                    EnvLoadFile;
fnPtrEnvSaveFile                    EnvSaveFile;
fnPtrEnvGetFkIdx                    EnvGetFkIdx;
fnPtrEnvSetFkIdx                    EnvSetFkIdx;
fnPtrEnvGetGeoIdx                   EnvGetGeoIdx;
fnPtrEnvSetGeoIdx                   EnvSetGeoIdx;
fnPtrEnvGetGeoStr                   EnvGetGeoStr;
fnPtrEnvSetGeoStr                   EnvSetGeoStr;
fnPtrEnvGetGeoConst                 EnvGetGeoConst;
fnPtrEnvGetFkConst                  EnvGetFkConst;
fnPtrEnvGetFkPtr                    EnvGetFkPtr;
fnPtrEnvSetEarthShape               EnvSetEarthShape;
fnPtrEnvGetEarthShape               EnvGetEarthShape;



void LoadEnvConstDll(const char* libPath);
void FreeEnvConstDll();




#endif
// ========================= End of auto generated code ==========================
