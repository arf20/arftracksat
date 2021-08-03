// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#ifndef TIMEFUNCDLL_H
#define TIMEFUNCDLL_H

#include "../services/DllUtils.h"

// Provide the path to the dll/so
#ifdef _WIN32
  #define TimeFuncDll "TimeFunc.dll"
#else
  #define TimeFuncDll "libtimefunc.so"
#endif


// Initializes the TimeFunc DLL for use in the program. 
// If this function returns an error, it is recommended that you stop the program immediately.
// 
// An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
// apPtr              The pointer that was returned from DllMain.DllMainInit (in-Long)
// returns Returns zero indicating the TimeFunc DLL has been initialized successfully. Other values indicate an error.
typedef int (STDCALL *fnPtrTimeFuncInit)(__int64 apPtr);


// Returns the information about the TimeFunc DLL.  The information is placed in the string parameter you pass in.
// The returned string provides information about the version number, build date, and the platform of the TimeFunc DLL.
// infoStr            A string to hold the information about TimeFunc.dll. (out-Character[128])
typedef void (STDCALL *fnPtrTimeFuncGetInfo)(char infoStr[128]);


// Loads timing constants data from an input file.
// Time constants can be included directly in the main input file or they can be read from a separate file identified with "TIMFIL=[pathname\filename]".
// 
// The input file is read in two passes. The function first looks for "TIMFIL=" lines, then it looks for timing constant data which was included directly. The result of this is that data entered using both methods will be processed, but the "TIMFIL=" data will be processed first.
// 
// The time constants are also read in from each VCM. However, only the most recent time constants among VCMs are stored in the memory, see VCM.dll documentation.
// See the "Time Constants Data Description" section in the accompanying TimeFunc documentation file for supported formats.
// tconFile           The name of the file from which to read timing constants data. (in-Character[512])
// returns 0 if the input file has been loaded successfully, non-0 if error.
typedef int (STDCALL *fnPtrTConLoadFile)(char tconFile[512]);


// Loads timing constants data and prediction control (6P-card) from an input file.
// Time constants can be included directly in the main input file or they can be read from a separate file identified with "TIMFIL=[pathname\filename]".
// 
// The input file is read in two passes. The function first looks for "TIMFIL=" lines, then it looks for timing constant data which was included directly. The result of this is that data entered using both methods will be processed, but the "TIMFIL=" data will be processed first.
// 
// The time constants are also read in from each VCM. However, only the most recent time constants among VCMs are stored in the memory, see VCM.dll documentation.
// tconFile           The name of the file from which to read timing constants data and/or prediction control data. (in-Character[512])
// returns 0 if the input file is loaded successfully, non-0 if there is an error.
typedef int (STDCALL *fnPtrTimeFuncLoadFile)(char tconFile[512]);


// Checks to see if timing constants have been loaded into memory.
// The timing constants can be loaded from a timing constants file or from VCM(s).  See TConLoadFile, TConAddOne, and TConAddARec functions.
// returns 1 if timing constants data have been loaded, some other value if not.
typedef int (STDCALL *fnPtrIsTConFileLoaded)();


// Saves currently loaded timing constants data to a file.
// The data will be saved in the format specified by the form parameter, regardless of the format or method originally used to load it.
// tconFile           The name of the file in which to save the timing constants data. (in-Character[512])
// saveMode           Specifies whether to create a new file or append to an existing one. (0 = Create new file, 1= Append to existing file) (in-Integer)
// saveForm           Specifies the format in which to save the file. (0 = SPECTER Print Record format, 1 = XML format (future implementation)) (in-Integer)
// returns 0 if the data is successfully saved to the file, non-0 if there is an error.
typedef int (STDCALL *fnPtrTConSaveFile)(char tconFile[512], int saveMode, int saveForm);


// Adds a timing constant record to memory. Note that this function is solely for backward compatible with legacy software.
// The users should use TConLoadFile or TimeFuncLoadFile to load timing constants file instead.
// refDs50UTC         Reference time (days since 1950, UTC) (in-Double)
// leapDs50UTC        Leap Second time (days since 1950, UTC) (in-Double)
// taiMinusUTC        TAI minus UTC offset at the reference time (seconds) (in-Double)
// ut1MinusUTC        UT1 minus UTC offset at the reference time (seconds) (in-Double)
// ut1Rate            UT1 rate of change versus UTC at the reference time (msec/day) (in-Double)
// polarX             Polar wander (X direction) at the reference time (arc-seconds) (in-Double)
// polarY             Polar wander (Y direction) at the reference time (arc-seconds) (in-Double)
// returns 0 if the timing constants record is successfully added to memory, non-0 if there is an error.
typedef int (STDCALL *fnPtrTConAddARec)(double refDs50UTC, double leapDs50UTC, double taiMinusUTC, double ut1MinusUTC, double ut1Rate, double polarX, double polarY);


// Adds one timing constant record to memory. This API can be used to avoid TConLoadFile's file I/O
// refDs50UTC         Reference time (days since 1950, UTC) (in-Double)
// taiMinusUTC        TAI minus UTC offset at the reference time (seconds) (in-Double)
// ut1MinusUTC        UT1 minus UTC offset at the reference time (seconds) (in-Double)
// ut1Rate            UT1 rate of change versus UTC at the reference time (msec/day) (in-Double)
// polarX             Polar wander (X direction) at the reference time (arc-seconds) (in-Double)
// polarY             Polar wander (Y direction) at the reference time (arc-seconds) (in-Double)
// returns 0 if the timing constants record is successfully added to memory, non-0 if there is an error.
typedef int (STDCALL *fnPtrTConAddOne)(double refDs50UTC, double taiMinusUTC, double ut1MinusUTC, double ut1Rate, double polarX, double polarY);


// Retrieves the timing constants record, if exists, at the requested input time in ds50UTC.
// If the requested record is not found, 0's will be returned for all of the constants. You can use this fact to determine whether the record was found or not. Simply check the taiMinusUTC value after calling this function. Since that value can never be 0 for a valid record, if it is 0 the record was not found.
// ds50UTC            Input days since 1950, UTC (in-Double)
// taiMinusUTC        Returned TAI minus UTC offset at requested time (seconds) (out-Double)
// ut1MinusUTC        Returned UT1 minus UTC offset at requested time (seconds) (out-Double)
// ut1Rate            Returned UT1 rate of change versus UTC at Reference time (msec/day) (out-Double)
// polarX             Returned interpolated polar wander (X direction) at requested time (arc-seconds) (out-Double)
// polarY             Returned interpolated polar wander (Y direction) at requested time (arc-seconds) (out-Double)
typedef void (STDCALL *fnPtrUTCToTConRec)(double ds50UTC, double* taiMinusUTC, double* ut1MinusUTC, double* ut1Rate, double* polarX, double* polarY);


// Removes all the timing constants records in memory.
// returns 0 if all timing constants records are successfully removed from memory, non-0 if there is an error.
typedef int (STDCALL *fnPtrTConRemoveAll)();


// Converts an internal time in ds50UTC to a string in DTG20 format. The resulting string takes the form "YYYY/DDD HHMM SS.SSS".
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956/001 0000 00.000".
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// dtg20              A string to hold the result of the conversion. (out-Character[20])
typedef void (STDCALL *fnPtrUTCToDTG20)(double ds50UTC, char dtg20[20]);


// Converts a time in ds50UTC to a time in DTG19 format. The resulting string takes the form "YYYYMonDDHHMMSS.SSS".
// See "UTCToDTG20" for an example.
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956Jan01000000.000".
// Note, the return value is in the DTG19 format "YYYYMonDDHHMMSS.SSS", not the "YY DDD HH MM SS.SSS" format.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// dtg19              A string to hold the result of the conversion. (out-Character[19])
typedef void (STDCALL *fnPtrUTCToDTG19)(double ds50UTC, char dtg19[19]);


// Converts a time in ds50UTC to a time in DTG17 format. The resulting string takes the form "YYYY/DDD.DDDDDDDD" format.
// See "UTCToDTG20" for an example.
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956/001.00000000".
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// dtg17              A string to hold the result of the conversion. (out-Character[17])
typedef void (STDCALL *fnPtrUTCToDTG17)(double ds50UTC, char dtg17[17]);


// Converts a time in ds50UTC to a time in DTG15 format. The resulting string takes the form "YYDDDHHMMSS.SSS".
// See "UTCToDTG20" for an example.
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "56001000000.000".
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// dtg15              A string to hold the result of the conversion. (out-Character[15])
typedef void (STDCALL *fnPtrUTCToDTG15)(double ds50UTC, char dtg15[15]);


// Converts a time in one of the DTG formats to a time in ds50UTC. DTG15, DTG17, DTG19, and DTG20 formats are accepted.
// See "UTCToDTG20" for an example.
// During the conversion, this function processes only numbers and the '.' character. This means that you can format dtgStr in a format that makes sense. You can use spaces and the '/' character for example if you wish.
// 
// The function can process dates from 1950 to 2049. Any input outside this range will yield "0.0".
// This function supports DTG19 inputs in both "YY DDD HH MM SS.SSS" and "YYYYMonDDHHMMSS.SSS" formats.
// dtg                The string to convert. Can be any of the DTG formats previously documented. (in-Character[20])
// returns The number of days since 1950, UTC. Partial days may be returned.
typedef double (STDCALL *fnPtrDTGToUTC)(char dtg[20]);


// Converts a time in ds50UTC to a time in ds50TAI using timing constants records in memory. 
// If no timing constants records were loaded, ds50UTC and ds50TAI are the same.
// Partial days may be returned.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// returns The number of days since 1950, TAI. Partial days may be returned.
typedef double (STDCALL *fnPtrUTCToTAI)(double ds50UTC);


// Converts a time in ds50UTC to a time in ds50UT1 using timing constants records in memory. 
// If no timing constants records were loaded, ds50UTC and ds50UT1 are the same. 
// Partial days may be returned.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// returns The number of days since 1950, UT1. Partial days may be returned.
typedef double (STDCALL *fnPtrUTCToUT1)(double ds50UTC);


// Converts a time in ds50UTC to a time in ds50ET using timing constants records in memory. 
// If no timing constants records were loaded, ds50UTC and ds50UT1 are the same. 
// Partial days may be returned.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// returns The number of days since 1950, ET. Partial days may be returned.
typedef double (STDCALL *fnPtrUTCToET)(double ds50UTC);


// Converts a time in ds50TAI to a time in ds50UTC using timing constants records in memory. 
// If no timing constants records were loaded, ds50TAI and ds50UTC are the same. 
// Partial days may be returned.
// ds50TAI            Days since 1950, TAI to be converted. (in-Double)
// returns The number of Days since 1950, UTC. Partial days may be returned.
typedef double (STDCALL *fnPtrTAIToUTC)(double ds50TAI);


// Converts a time in ds50TAI to a time in ds50UT1 using timing constants records in memory. 
// If no timing constants records were loaded, ds50TAI and ds50UT1 are the same. 
// Partial days may be returned.
// ds50TAI            Days since 1950, TAI to be converted. (in-Double)
// returns The number of days since 1950, UT1. Partial days may be returned.
typedef double (STDCALL *fnPtrTAIToUT1)(double ds50TAI);


// Converts a year and a number of days to a time in ds50UTC. 
// Partial days may be returned.
// year               Two or four digit years are accepted. (in-Integer)
// dayOfYear          The day of year. Partial days can be specified. (in-Double)
// returns The number of days since 1950, UTC. Partial days may be returned.
typedef double (STDCALL *fnPtrYrDaysToUTC)(int year, double dayOfYear);


// Converts a time in ds50UTC to a year and day of year.
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield Year=1956, Day=1.0.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// year               A reference to a variable in which to place the 4-digit year. (out-Integer)
// dayOfYear          A reference to a variable in which to place the day of year. Partial days may be expressed in this variable. (out-Double)
typedef void (STDCALL *fnPtrUTCToYrDays)(double ds50UTC, int* year, double* dayOfYear);


// Converts a set of time components (year, day of year, hour, minute, second) to a time in ds50UTC. 
// Partial days may be returned.
// See "TimeComps2ToUTC" for a function which takes a month and day instead of a day of year value.
// year               Two or four digit years are accepted. (in-Integer)
// dayOfYear          The day of year, expressed as a whole number. (in-Integer)
// hh                 The hour. (in-Integer)
// mm                 The minute. (in-Integer)
// sss                The second, including partial seconds if desired. (in-Double)
// returns The number of Days since 1950, UTC. Partial days may be returned.
typedef double (STDCALL *fnPtrTimeComps1ToUTC)(int year, int dayOfYear, int hh, int mm, double sss);


// Converts a time in ds50UTC to its individual components (year, day of year, hour, minute, second).
// See "TimeComps1ToUTC" for an example.
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will be reset to that value.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// year               A reference to a variable in which to store the 4-digit year. (out-Integer)
// dayOfYear          A reference to a variable in which to store the day of year. (out-Integer)
// hh                 A reference to a variable in which to store the hour. (out-Integer)
// mm                 A reference to a variable in which to store the minute. (out-Integer)
// sss                A reference to a variable in which to store the second. Partial seconds may be expressed if necessary. (out-Double)
typedef void (STDCALL *fnPtrUTCToTimeComps1)(double ds50UTC, int* year, int* dayOfYear, int* hh, int* mm, double* sss);


// Converts a set of time components (year, month, day of month, hour, minute, second) to a time in ds50UTC. 
// Partial days may be returned.
// See "TimeComps1ToUTC" for a function which takes a day of year value instead of a month and day.
// year               Two or four digit years are accepted. (in-Integer)
// mon                The month. (in-Integer)
// dayOfMonth         The day of the month. (in-Integer)
// hh                 The hour. (in-Integer)
// mm                 The minute. (in-Integer)
// sss                The second. (in-Double)
// returns The number of Days since 1950, UTC. Partial days may be returned.
typedef double (STDCALL *fnPtrTimeComps2ToUTC)(int year, int mon, int dayOfMonth, int hh, int mm, double sss);


// Converts a time in ds50UTC to its individual components (year, month, day of month, hour, minute, second).
// See "TimeComps1ToUTC" for an example.
// The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will be reset to that value.
// ds50UTC            Days since 1950, UTC to be converted. (in-Double)
// year               A reference to a variable in which to store the 4-digit year. (out-Integer)
// month              A reference to a variable in which to store the month. (out-Integer)
// dayOfMonth         A reference to a variable in which to store the day of the month. (out-Integer)
// hh                 A reference to a variable in which to store the hour. (out-Integer)
// mm                 A reference to a variable in which to store the minute. (out-Integer)
// sss                A reference to a variable in which to store the second. Partial seconds may be expressed if necessary. (out-Double)
typedef void (STDCALL *fnPtrUTCToTimeComps2)(double ds50UTC, int* year, int* month, int* dayOfMonth, int* hh, int* mm, double* sss);


// Computes right ascension of Greenwich at the specified time in ds50UT1. 
// The Fk constants as you currently have them set up in EnvConst.dll are used.
// EnvConst.dll is not marked as a direct dependency of TimeFunc.dll. However, it obviously must be loaded in order to be able to use this function since you must first obtain a handle via the EnvGetFkPtr() function.
// ds50UT1            Input days since 1950, UT1. (in-Double)
// envFk              A handle to the FK data. Use the value returned from EnvGetFkPtr(), located in EnvConst.dll. (in-Long)
// returns Right ascension of Greenwich in radians at the specified time.
typedef double (STDCALL *fnPtrThetaGrnwch)(double ds50UT1, __int64 envFk);


// Computes right ascension of Greenwich at the specified time in ds50UT1 using the Fourth Fundamental Catalogue (FK4).
// There is no need to load or initialize EnvConst.dll when computing right ascension using this function.
// ds50UT1            Days since 1950, UT1. (in-Double)
// returns Right ascension of Greenwich in radians at the specified time using FK4.
typedef double (STDCALL *fnPtrThetaGrnwchFK4)(double ds50UT1);


// Computes right ascension of Greenwich at the specified time in ds50UT1 using the Fifth Fundamental Catalogue (FK5).
// There is no need to load or initialize EnvConst.dll when computing right ascension using this function.
// ds50UT1            Input days since 1950, UT1. (in-Double)
// returns Right ascension of Greenwich in radians at the specified time using FK5.
typedef double (STDCALL *fnPtrThetaGrnwchFK5)(double ds50UT1);


// This function is intended for future use.  No information is currently available.
// This function is intended for future use.  No information is currently available.
// funcIdx            Input function index (in-Integer)
// frArr              Input (in-Double[*])
// toArr              Output (out-Double[*])
typedef void (STDCALL *fnPtrTimeConvFrTo)(int funcIdx, double frArr[], double toArr[]);


// Returns prediction control parameters. The parameters are placed in the reference variables passed to this function.
// startFrEpoch       Indicates whether startTime is expressed in minutes since epoch. (startFrEpoch = 1: startTime is in minutes since epoch, startFrEpoch = 0: startTime is in days since 1950, UTC) (out-Integer)
// stopFrEpoch        Indicates whether stopTime is expressed in minutes since epoch. (stopFrEpoch = 1: stopTime is in minutes since epoch, stopFrEpoch = 0: stopTime is in days since 1950, UTC) (out-Integer)
// startTime          The start time. Depending on the value of startFrEpoch, start time can be expressed in minutes since epoch or days since 1950, UTC. (out-Double)
// stopTime           The stop time. Depending on the value of stopFrEpoch, stop time can be expressed in minutes since epoch or days since 1950, UTC. (out-Double)
// interval           The Step size (min). (out-Double)
typedef void (STDCALL *fnPtrGet6P)(int* startFrEpoch, int* stopFrEpoch, double* startTime, double* stopTime, double* interval);


// Sets prediction control parameters.
// startFrEpoch       Indicates whether startTime is expressed in minutes since epoch. (startFrEpoch = 1: Value of startTime is in minutes since epoch, startFrEpoch = 0: Value of startTime is in days since 1950, UTC) (in-Integer)
// stopFrEpoch        Indicates whether stopTime is expressed in minutes since epoch. (stopFrEpoch = 1: Value of stopTime is in minutes since epoch, stopFrEpoch = 0: Value of stopTime is in days since 1950, UTC) (in-Integer)
// startTime          Start time. (in-Double)
// stopTime           Stop time. (in-Double)
// interval           Step size (min). (in-Double)
typedef void (STDCALL *fnPtrSet6P)(int startFrEpoch, int stopFrEpoch, double startTime, double stopTime, double interval);


// Returns current prediction control parameters in form of a 6P-Card string.
// card6PLine         The resulting 6P-Card string. (out-Character[512])
typedef void (STDCALL *fnPtrGet6PCardLine)(char card6PLine[512]);
  



// TimeFuncDll's function pointers
fnPtrTimeFuncInit                   TimeFuncInit;
fnPtrTimeFuncGetInfo                TimeFuncGetInfo;
fnPtrTConLoadFile                   TConLoadFile;
fnPtrTimeFuncLoadFile               TimeFuncLoadFile;
fnPtrIsTConFileLoaded               IsTConFileLoaded;
fnPtrTConSaveFile                   TConSaveFile;
fnPtrTConAddARec                    TConAddARec;
fnPtrTConAddOne                     TConAddOne;
fnPtrUTCToTConRec                   UTCToTConRec;
fnPtrTConRemoveAll                  TConRemoveAll;
fnPtrUTCToDTG20                     UTCToDTG20;
fnPtrUTCToDTG19                     UTCToDTG19;
fnPtrUTCToDTG17                     UTCToDTG17;
fnPtrUTCToDTG15                     UTCToDTG15;
fnPtrDTGToUTC                       DTGToUTC;
fnPtrUTCToTAI                       UTCToTAI;
fnPtrUTCToUT1                       UTCToUT1;
fnPtrUTCToET                        UTCToET;
fnPtrTAIToUTC                       TAIToUTC;
fnPtrTAIToUT1                       TAIToUT1;
fnPtrYrDaysToUTC                    YrDaysToUTC;
fnPtrUTCToYrDays                    UTCToYrDays;
fnPtrTimeComps1ToUTC                TimeComps1ToUTC;
fnPtrUTCToTimeComps1                UTCToTimeComps1;
fnPtrTimeComps2ToUTC                TimeComps2ToUTC;
fnPtrUTCToTimeComps2                UTCToTimeComps2;
fnPtrThetaGrnwch                    ThetaGrnwch;
fnPtrThetaGrnwchFK4                 ThetaGrnwchFK4;
fnPtrThetaGrnwchFK5                 ThetaGrnwchFK5;
fnPtrTimeConvFrTo                   TimeConvFrTo;
fnPtrGet6P                          Get6P;
fnPtrSet6P                          Set6P;
fnPtrGet6PCardLine                  Get6PCardLine;



void LoadTimeFuncDll(const char* libPath);
void FreeTimeFuncDll();




#endif
// ========================= End of auto generated code ==========================
