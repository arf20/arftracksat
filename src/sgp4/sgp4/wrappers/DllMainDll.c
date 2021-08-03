// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#include "../services/DllUtils.h"
#include "DllMainDll.h"


static void* hLib = NULL; // handle to the dll

// Load and assign function pointers
void LoadDllMainDll(const char* libPath)
{
   char wholeLibPath[128];

   sprintf(wholeLibPath, "%s%s", libPath, DllMainDll);

   // Explicitly load the dll
   hLib = LoadLib(wholeLibPath);


   // Assign function pointers to the appropriate dll funtions
   MacroGetFnPtr(hLib, DllMainInit);
   MacroGetFnPtr(hLib, DllMainGetInfo);
   MacroGetFnPtr(hLib, OpenLogFile);
   MacroGetFnPtr(hLib, CloseLogFile);
   MacroGetFnPtr(hLib, LogMessage);
   MacroGetFnPtr(hLib, GetLastErrMsg);
   MacroGetFnPtr(hLib, GetLastInfoMsg);
   MacroGetFnPtr(hLib, GetInitDllNames);
   MacroGetFnPtr(hLib, TestInterface);
}



// Free DllMainDll
void FreeDllMainDll()
{
   FreeLib(hLib, DllMainDll);
}
// ========================= End of auto generated code ==========================
