#include <math.h>
#include "DllUtils.h"
#include "DllMainDll_Service.h"

#include "../wrappers/DllMainDll.h"


// Show last logged error message
void ShowErrMsg()
{
   char  errMsg[LOGMSGLEN];

   GetLastErrMsg(errMsg);
   errMsg[LOGMSGLEN - 1] = 0;
   printf("%s\n", errMsg);
}

// Show last logged error message
void ShowInfoMsg()
{
   char  infoMsg[LOGMSGLEN];

   GetLastInfoMsg(infoMsg);
   infoMsg[LOGMSGLEN - 1] = 0;
   printf("%s\n", infoMsg);
}


// Show last logged error message and exit
void ShowMsgAndTerminate()
{
   ShowErrMsg();
   exit(1);
}

// Show last logged error message
void WriteErrMsg(FILE* fp)
{
   char  errMsg[LOGMSGLEN];

   GetLastErrMsg(errMsg);
   errMsg[LOGMSGLEN - 1] = 0;
   fprintf(fp, "%s\n", errMsg);
}


// Convert minutes since satellite's epoch to ds50UTC
double MseToDs50UTC(double epochDs50UTC, double mse)
{
   return   epochDs50UTC + (mse / 1440.0);
}

// Convert days to whole miliseconds
double DaysInWholeMilisec(double ds50)
{
   return (double)((__int64)(ds50 * MSPERDAY) / MSPERDAY);
}

// Return satellite number from a satKey
int SatKeyToSatNum(__int64 satKey)
{
   return (int)((satKey % (int)1E8) / 10);
}


// Return sensor number from obsKey
int ObsKeyToSenNum(__int64 obsKey)
{
   return (int)(obsKey % 1000);
}

// Return satellite number from a senSatKey
int SenSatKeyToSatNum(__int64 senSatKey)
{
   return senSatKey % (int)1E9;
}

// Retrun sensor number from a senSatKey
int SenSatKeyToSenNum(__int64 senSatKey)
{
   return (int)(senSatKey / 1E9);
}




// Determine if the input value is small enough to be displayed as 0.000 instead of -0.000
int PrintAsZero(double inValue)
{
   if (inValue >= 0)
      return 0;
   else if (fabs(inValue * 10000.0) < 5.0)
      return 1;
   else
      return 0;
}

// Determine if the input value is small enough to be displayed as 0.000 instead of -0.000
double NonNegativeZero(double inValue, int decPlaces)
{
   if (inValue <= 0 && (-inValue * pow(10.0, decPlaces + 1) < 5.0))
      return 0;
   else
      return inValue;
}


// Retrieve double value of a field of an object (32bit integer ID)
double GetKey32DoubleField(fnPtrGetKey32Field fName, int key, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(key, idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atof(valueStr);
}


// Retrieve integer value of a field of an object (32bit integer key)
int GetKey32IntegerField(fnPtrGetKey32Field fName, int key, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(key, idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atoi(valueStr);
}


// Retrieve char value of a field of an object (32bit integer key)
char GetKey32CharField(fnPtrGetKey32Field fName, int key, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(key, idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return valueStr[0];
}

// Retrieve double value of a field of an object (64bit integer key)
double GetKey64DoubleField(fnPtrGetKey64Field fName, __int64 key, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(key, idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atof(valueStr);
}


// Retrieve integer value of a field of an object (64bit integer key)
int GetKey64IntegerField(fnPtrGetKey64Field fName, __int64 key, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(key, idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atoi(valueStr);
}


// Retrieve char value of a field of an object (64bit integer key)
char GetKey64CharField(fnPtrGetKey64Field fName, __int64 key, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(key, idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atoi(valueStr);
}


// Retrieve double value of a field specified by function name and field index
double GetDoubleField(fnPtrGetField fName, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atof(valueStr);
}


// Retrieve integer value of a field specified by function name and field index
int GetIntegerField(fnPtrGetField fName, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return atoi(valueStr);
}


// Retrieve char value of a field specified by function name and field index
char GetCharField(fnPtrGetField fName, int idx)
{
   char valueStr[GETSETSTRLEN];

   fName(idx, valueStr);
   valueStr[GETSETSTRLEN - 1] = 0;

   return valueStr[0];
}