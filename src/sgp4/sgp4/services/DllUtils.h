#ifndef DLLUTILS_H
#define DLLUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Disable warning messages about strcpy
#pragma warning(disable : 4996)
#pragma warning(disable : 4133)


//Directives for Windows PC and UNIX platforms
#ifdef _WIN32
   #include <windows.h>
   //#define STDCALL __stdcall // Remember to use this __stdcall in VC++ 6.0
   #define STDCALL __cdecl   // Remember to select this __cdecl in VS 2008
   // windows platforms has __int64 type - 64-bit integer
#else
   #include <dlfcn.h>
   #define STDCALL 
   // unix-family platforms doesn't support __int64, therefore need to define it here
   typedef long long __int64;
#endif


// Use 64-bit integer for fortran addresses
typedef __int64 fAddr;

// Create macro to combine assigning function pointer and error checking in one
#define MacroGetFnPtr(hLib, fName)  fName = (fnPtr##fName) GetFnPtr(hLib, #fName)


#ifndef _WIN32
   // Macro for finding max, min
   #define max(x, y) ((x) > (y) ? (x) : (y))
   #define min(x, y) ((x) < (y) ? (x) : (y))
#endif

// Generic function pointer for all Get functions
typedef void (STDCALL *fnPtrGetKey32Field)(int key, int fieldIdx, char strValue[512]);
typedef void (STDCALL *fnPtrGetKey64Field)(__int64 key, int fieldIdx, char strValue[512]);
typedef void (STDCALL *fnPtrGetField)(int fieldIdx, char strValue[512]);



#define PI        3.1415926535897932384626
#define MPD       1440.0
#define MSPERDAY  86400000
#define TODEG     180.0 / PI
#define TORAD     PI / 180.0

// Command line arguments structure
typedef struct
{
   char  inFile[512];         // Input file name from command line
   char  outFile[512];        // Output file name from command line
   char  logFile[512];        // Log file name from command line
   char  bConst[7];           // Optional earth constant from command line   
   char  libPath[128];        // Path to Astrodynamic Standards Dlls/SOs
   char  sgp4LicPath[128];    // Path to the sgp4 license key file "SGP4_Open_License.txt"
} CommandLineArgs;


void   GetCommandLineArgs(int argc, char* argv[], const char* exeName, const char* appName, CommandLineArgs* cla);
void*  LoadLib(const char* dllName);
void*  GetFnPtr(void* hLib, char* fName);
char*  ToStrLower(char* inStr);
void   FreeLib(void* hLib, const char* dllName);
FILE*  FileOpen(char* fileName, char* mode);
void   Rtrim(char* fileName);
char*  GetOneJob(FILE* inputFilePtr, int* isEndAll);
void   PrintWarning(FILE* fp, const char* softwareName);
double DaysInWholeMilisec(double ds50);






#endif
