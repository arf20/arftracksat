// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#include "../services/DllUtils.h"
#include "TleDll.h"


static void* hLib = NULL; // handle to the dll

// Load and assign function pointers
void LoadTleDll(const char* libPath)
{
   char wholeLibPath[128];

   sprintf(wholeLibPath, "%s%s", libPath, TleDll);

   // Explicitly load the dll
   hLib = LoadLib(wholeLibPath);


   // Assign function pointers to the appropriate dll funtions
   MacroGetFnPtr(hLib, TleInit);
   MacroGetFnPtr(hLib, TleGetInfo);
   MacroGetFnPtr(hLib, TleLoadFile);
   MacroGetFnPtr(hLib, TleSaveFile);
   MacroGetFnPtr(hLib, TleRemoveSat);
   MacroGetFnPtr(hLib, TleRemoveAllSats);
   MacroGetFnPtr(hLib, TleGetCount);
   MacroGetFnPtr(hLib, TleGetLoaded);
   MacroGetFnPtr(hLib, TleAddSatFrLines);
   MacroGetFnPtr(hLib, TleAddSatFrLinesML);
   MacroGetFnPtr(hLib, TleAddSatFrCsv);
   MacroGetFnPtr(hLib, TleAddSatFrCsvML);
   MacroGetFnPtr(hLib, TleAddSatFrFieldsGP);
   MacroGetFnPtr(hLib, TleAddSatFrFieldsGP2);
   MacroGetFnPtr(hLib, TleAddSatFrFieldsGP2ML);
   MacroGetFnPtr(hLib, TleUpdateSatFrFieldsGP);
   MacroGetFnPtr(hLib, TleUpdateSatFrFieldsGP2);
   MacroGetFnPtr(hLib, TleAddSatFrFieldsSP);
   MacroGetFnPtr(hLib, TleAddSatFrFieldsSPML);
   MacroGetFnPtr(hLib, TleUpdateSatFrFieldsSP);
   MacroGetFnPtr(hLib, TleSetField);
   MacroGetFnPtr(hLib, TleGetField);
   MacroGetFnPtr(hLib, TleGetAllFieldsGP);
   MacroGetFnPtr(hLib, TleGetAllFieldsGP2);
   MacroGetFnPtr(hLib, TleGetAllFieldsSP);
   MacroGetFnPtr(hLib, TleParseGP);
   MacroGetFnPtr(hLib, TleLinesToArray);
   MacroGetFnPtr(hLib, TleParseSP);
   MacroGetFnPtr(hLib, TleGetLines);
   MacroGetFnPtr(hLib, TleGetCsv);
   MacroGetFnPtr(hLib, TleGPFieldsToLines);
   MacroGetFnPtr(hLib, TleGPFieldsToCsv);
   MacroGetFnPtr(hLib, TleGPArrayToLines);
   MacroGetFnPtr(hLib, TleGPArrayToCsv);
   MacroGetFnPtr(hLib, TleSPFieldsToLines);
   MacroGetFnPtr(hLib, TleGetSatKey);
   MacroGetFnPtr(hLib, TleGetSatKeyML);
   MacroGetFnPtr(hLib, TleFieldsToSatKey);
   MacroGetFnPtr(hLib, TleFieldsToSatKeyML);
   MacroGetFnPtr(hLib, TleAddSatFrArray);
   MacroGetFnPtr(hLib, TleAddSatFrArrayML);
   MacroGetFnPtr(hLib, TleUpdateSatFrArray);
   MacroGetFnPtr(hLib, TleDataToArray);
   MacroGetFnPtr(hLib, TleLinesToCsv);
   MacroGetFnPtr(hLib, TleCsvToLines);
   MacroGetFnPtr(hLib, SetTleKeyMode);
   MacroGetFnPtr(hLib, GetTleKeyMode);
}



// Free TleDll
void FreeTleDll()
{
   FreeLib(hLib, TleDll);
}
// ========================= End of auto generated code ==========================
