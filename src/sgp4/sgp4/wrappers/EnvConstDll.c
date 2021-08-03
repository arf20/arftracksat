// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#include "../services/DllUtils.h"
#include "EnvConstDll.h"


static void* hLib = NULL; // handle to the dll

// Load and assign function pointers
void LoadEnvConstDll(const char* libPath)
{
   char wholeLibPath[128];

   sprintf(wholeLibPath, "%s%s", libPath, EnvConstDll);

   // Explicitly load the dll
   hLib = LoadLib(wholeLibPath);


   // Assign function pointers to the appropriate dll funtions
   MacroGetFnPtr(hLib, EnvInit);
   MacroGetFnPtr(hLib, EnvGetInfo);
   MacroGetFnPtr(hLib, EnvLoadFile);
   MacroGetFnPtr(hLib, EnvSaveFile);
   MacroGetFnPtr(hLib, EnvGetFkIdx);
   MacroGetFnPtr(hLib, EnvSetFkIdx);
   MacroGetFnPtr(hLib, EnvGetGeoIdx);
   MacroGetFnPtr(hLib, EnvSetGeoIdx);
   MacroGetFnPtr(hLib, EnvGetGeoStr);
   MacroGetFnPtr(hLib, EnvSetGeoStr);
   MacroGetFnPtr(hLib, EnvGetGeoConst);
   MacroGetFnPtr(hLib, EnvGetFkConst);
   MacroGetFnPtr(hLib, EnvGetFkPtr);
   MacroGetFnPtr(hLib, EnvSetEarthShape);
   MacroGetFnPtr(hLib, EnvGetEarthShape);
}



// Free EnvConstDll
void FreeEnvConstDll()
{
   FreeLib(hLib, EnvConstDll);
}
// ========================= End of auto generated code ==========================
