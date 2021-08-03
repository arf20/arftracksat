#ifndef DLLMAINDLL_SERVICE_H
#define DLLMAINDLL_SERVICE_H

#include <stdio.h>
#include "DllUtils.h"

//error code 0 = OK, 1 = Warning, 2 = Error
#define  IDX_ERR_NONE   0
#define  IDX_ERR_WARN   1
#define  IDX_ERR_ERROR  2

void ShowErrMsg();
void ShowInfoMsg();
void WriteErrMsg(FILE* fp);
void ShowMsgAndTerminate();
void FreeDllMainDll();
int  SatKeyToSatNum(__int64 satKey);
double MseToDs50UTC(double epochDs50UTC, double mse);
int    ObsKeyToSenNum(__int64 obsKey);
int    SenSatKeyToSatNum(__int64 senSatKey);
int    SenSatKeyToSenNum(__int64 senSatKey);


int    PrintAsZero(double intValue);
double NonNegativeZero(double inValue, int decPlaces);

double GetKey32DoubleField(fnPtrGetKey32Field fName, int key, int idx);
int    GetKey32IntegerField(fnPtrGetKey32Field fName, int key, int idx);
char   GetKey32CharField(fnPtrGetKey32Field fName, int key, int idx);
double GetKey64DoubleField(fnPtrGetKey64Field fName, __int64 key, int idx);
int    GetKey64IntegerField(fnPtrGetKey64Field fName, __int64 key, int idx);
char   GetKey64CharField(fnPtrGetKey64Field fName, __int64 key, int idx);
double GetDoubleField(fnPtrGetField fName, int idx);
int    GetIntegerField(fnPtrGetField fName, int idx);
char   GetCharField(fnPtrGetField fName, int idx);



#endif
