// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#include "../services/DllUtils.h"
#include "Sgp4PropDll.h"


static void* hLib = NULL; // handle to the dll

// Load and assign function pointers
void LoadSgp4PropDll(const char* libPath)
{
   char wholeLibPath[128];

   sprintf(wholeLibPath, "%s%s", libPath, Sgp4PropDll);

   // Explicitly load the dll
   hLib = LoadLib(wholeLibPath);


   // Assign function pointers to the appropriate dll funtions
   MacroGetFnPtr(hLib, Sgp4Init);
   MacroGetFnPtr(hLib, Sgp4GetInfo);
   MacroGetFnPtr(hLib, Sgp4InitSat);
   MacroGetFnPtr(hLib, Sgp4RemoveSat);
   MacroGetFnPtr(hLib, Sgp4RemoveAllSats);
   MacroGetFnPtr(hLib, Sgp4GetCount);
   MacroGetFnPtr(hLib, Sgp4PropMse);
   MacroGetFnPtr(hLib, Sgp4PropDs50UTC);
   MacroGetFnPtr(hLib, Sgp4PropDs50UtcPosVel);
   MacroGetFnPtr(hLib, Sgp4PropDs50UtcLLH);
   MacroGetFnPtr(hLib, Sgp4PropDs50UtcPos);
   MacroGetFnPtr(hLib, Sgp4GetPropOut);
   MacroGetFnPtr(hLib, Sgp4PropAll);
   MacroGetFnPtr(hLib, Sgp4PosVelToKep);
   MacroGetFnPtr(hLib, Sgp4PosVelToTleArr);
   MacroGetFnPtr(hLib, Sgp4ReepochTLE);
   MacroGetFnPtr(hLib, Sgp4ReepochCsv);
   MacroGetFnPtr(hLib, Sgp4SetLicFilePath);
   MacroGetFnPtr(hLib, Sgp4GetLicFilePath);
   MacroGetFnPtr(hLib, Sgp4GenEphems);
   MacroGetFnPtr(hLib, Sgp4GenEphems_OS);
   MacroGetFnPtr(hLib, Sgp4PropAllSats);

   // "SGP4_Open_License.txt" is in the same folder as the Sgp4Prop.dll
   Sgp4SetLicFilePath(libPath);
}



// Free Sgp4PropDll
void FreeSgp4PropDll()
{
   FreeLib(hLib, Sgp4PropDll);
}
// ========================= End of auto generated code ==========================
