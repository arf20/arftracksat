// This wrapper file was generated automatically by the A3\GenDllWrappers program.

#include "../services/DllUtils.h"
#include "AstroFuncDll.h"


static void* hLib = NULL; // handle to the dll

// Load and assign function pointers
void LoadAstroFuncDll(const char* libPath)
{
   char wholeLibPath[128];

   sprintf(wholeLibPath, "%s%s", libPath, AstroFuncDll);

   // Explicitly load the dll
   hLib = LoadLib(wholeLibPath);


   // Assign function pointers to the appropriate dll funtions
   MacroGetFnPtr(hLib, AstroFuncInit);
   MacroGetFnPtr(hLib, AstroFuncGetInfo);
   MacroGetFnPtr(hLib, KepToEqnx);
   MacroGetFnPtr(hLib, KepToPosVel);
   MacroGetFnPtr(hLib, KepToUVW);
   MacroGetFnPtr(hLib, ClassToEqnx);
   MacroGetFnPtr(hLib, EqnxToClass);
   MacroGetFnPtr(hLib, EqnxToKep);
   MacroGetFnPtr(hLib, EqnxToPosVel);
   MacroGetFnPtr(hLib, PosVelToEqnx);
   MacroGetFnPtr(hLib, PosVelMuToEqnx);
   MacroGetFnPtr(hLib, PosVelToKep);
   MacroGetFnPtr(hLib, PosVelMuToKep);
   MacroGetFnPtr(hLib, PosVelToUUVW);
   MacroGetFnPtr(hLib, PosVelToPTW);
   MacroGetFnPtr(hLib, SolveKepEqtn);
   MacroGetFnPtr(hLib, CompTrueAnomaly);
   MacroGetFnPtr(hLib, NToA);
   MacroGetFnPtr(hLib, AToN);
   MacroGetFnPtr(hLib, KozaiToBrouwer);
   MacroGetFnPtr(hLib, BrouwerToKozai);
   MacroGetFnPtr(hLib, KepOscToMean);
   MacroGetFnPtr(hLib, XYZToLLH);
   MacroGetFnPtr(hLib, XYZToLLHTime);
   MacroGetFnPtr(hLib, LLHToXYZ);
   MacroGetFnPtr(hLib, LLHToXYZTime);
   MacroGetFnPtr(hLib, EFGToECI);
   MacroGetFnPtr(hLib, EFGToECITime);
   MacroGetFnPtr(hLib, ECIToEFG);
   MacroGetFnPtr(hLib, ECIToEFGTime);
   MacroGetFnPtr(hLib, ECRToEFG);
   MacroGetFnPtr(hLib, ECRToEFGTime);
   MacroGetFnPtr(hLib, EFGToECR);
   MacroGetFnPtr(hLib, EFGToECRTime);
   MacroGetFnPtr(hLib, EFGPosToLLH);
   MacroGetFnPtr(hLib, LLHToEFGPos);
   MacroGetFnPtr(hLib, RotJ2KToDate);
   MacroGetFnPtr(hLib, RotDateToJ2K);
   MacroGetFnPtr(hLib, CompSunMoonPos);
   MacroGetFnPtr(hLib, CompSunPos);
   MacroGetFnPtr(hLib, CompMoonPos);
   MacroGetFnPtr(hLib, AstroConvFrTo);
   MacroGetFnPtr(hLib, RADecToLAD);
   MacroGetFnPtr(hLib, AzElToLAD);
   MacroGetFnPtr(hLib, ECIToTopoComps);
   MacroGetFnPtr(hLib, RaDecToAzEl);
   MacroGetFnPtr(hLib, RaDecToAzElTime);
   MacroGetFnPtr(hLib, AzElToRaDec);
   MacroGetFnPtr(hLib, AzElToRaDecTime);
   MacroGetFnPtr(hLib, RAEToECI);
   MacroGetFnPtr(hLib, GetInitialDrag);
   MacroGetFnPtr(hLib, CovMtxPTWToUVW);
   MacroGetFnPtr(hLib, CovMtxUVWToPTW);
   MacroGetFnPtr(hLib, EarthObstructionAngles);
   MacroGetFnPtr(hLib, IsPointSunlit);
   MacroGetFnPtr(hLib, RotRADecl);
   MacroGetFnPtr(hLib, RotRADec_DateToEqnx);
   MacroGetFnPtr(hLib, RotRADec_EqnxToDate);
}



// Free AstroFuncDll
void FreeAstroFuncDll()
{
   FreeLib(hLib, AstroFuncDll);
}
// ========================= End of auto generated code ==========================
