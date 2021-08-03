// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#include "../services/DllUtils.h"
#include "TimeFuncDll.h"


static void* hLib = NULL; // handle to the dll

// Load and assign function pointers
void LoadTimeFuncDll(const char* libPath)
{
   char wholeLibPath[128];

   sprintf(wholeLibPath, "%s%s", libPath, TimeFuncDll);

   // Explicitly load the dll
   hLib = LoadLib(wholeLibPath);


   // Assign function pointers to the appropriate dll funtions
   MacroGetFnPtr(hLib, TimeFuncInit);
   MacroGetFnPtr(hLib, TimeFuncGetInfo);
   MacroGetFnPtr(hLib, TConLoadFile);
   MacroGetFnPtr(hLib, TimeFuncLoadFile);
   MacroGetFnPtr(hLib, IsTConFileLoaded);
   MacroGetFnPtr(hLib, TConSaveFile);
   MacroGetFnPtr(hLib, TConAddARec);
   MacroGetFnPtr(hLib, TConAddOne);
   MacroGetFnPtr(hLib, UTCToTConRec);
   MacroGetFnPtr(hLib, TConRemoveAll);
   MacroGetFnPtr(hLib, UTCToDTG20);
   MacroGetFnPtr(hLib, UTCToDTG19);
   MacroGetFnPtr(hLib, UTCToDTG17);
   MacroGetFnPtr(hLib, UTCToDTG15);
   MacroGetFnPtr(hLib, DTGToUTC);
   MacroGetFnPtr(hLib, UTCToTAI);
   MacroGetFnPtr(hLib, UTCToUT1);
   MacroGetFnPtr(hLib, UTCToET);
   MacroGetFnPtr(hLib, TAIToUTC);
   MacroGetFnPtr(hLib, TAIToUT1);
   MacroGetFnPtr(hLib, YrDaysToUTC);
   MacroGetFnPtr(hLib, UTCToYrDays);
   MacroGetFnPtr(hLib, TimeComps1ToUTC);
   MacroGetFnPtr(hLib, UTCToTimeComps1);
   MacroGetFnPtr(hLib, TimeComps2ToUTC);
   MacroGetFnPtr(hLib, UTCToTimeComps2);
   MacroGetFnPtr(hLib, ThetaGrnwch);
   MacroGetFnPtr(hLib, ThetaGrnwchFK4);
   MacroGetFnPtr(hLib, ThetaGrnwchFK5);
   MacroGetFnPtr(hLib, TimeConvFrTo);
   MacroGetFnPtr(hLib, Get6P);
   MacroGetFnPtr(hLib, Set6P);
   MacroGetFnPtr(hLib, Get6PCardLine);
}



// Free TimeFuncDll
void FreeTimeFuncDll()
{
   FreeLib(hLib, TimeFuncDll);
}
// ========================= End of auto generated code ==========================
