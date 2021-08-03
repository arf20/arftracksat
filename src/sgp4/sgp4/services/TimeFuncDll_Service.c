#include "DllUtils.h"
#include "TimeFuncDll_Service.h"

#include "../wrappers/TimeFuncDll.h"

// Time Conversion Functions
// CONVERT DAYS SINCE 1950 TO YYYY/DDD HHMM SS.SSS
char* UTCToDtg20Str(double ds50UTC)
{
   static char dtg20[21];

   UTCToDTG20(ds50UTC, dtg20);
   dtg20[20] = 0;

   return dtg20;
}

// Time Conversion Functions
// CONVERT DAYS SINCE 1950 TO YY/DDD HHMM SS.SSS
char* UTCToDtg18Str(double ds50UTC)
{
   static char dtg20[21];

   UTCToDTG20(ds50UTC, dtg20);
   dtg20[20] = 0;

   return &dtg20[2];
}


// CONVERT DAYS SINCE 1950 TO YYYYMonDDHHMMSS.SSS
char* UTCToDtg19Str(double ds50UTC)
{
   static char dtg19[20];

   UTCToDTG19(ds50UTC, dtg19);
   dtg19[19] = 0;
   
   return dtg19;
}


// CONVERT DAYS SINCE 1950 TO YYYY/DDD.DDDDDDDD
char* UTCToDtg17Str(double ds50UTC)
{
   static char dtg17[18];

   UTCToDTG17(ds50UTC, dtg17);
   dtg17[17] = 0;

   return dtg17;
}


// CONVERT DAYS SINCE 1950 TO 'YYDDDHHMMSS.SSS'
char* UTCToDtg15Str(double ds50UTC)
{
   static char dtg15[16];

   UTCToDTG15(ds50UTC, dtg15);
   dtg15[15] = 0;

   return dtg15;
}


// CONVERT DAYS SINCE 1950 TO 'YY DDD HH MM SS.SSSSSSS'
char* UTCToDtg23Str(double ds50UTC)
{
   static char dtg23[24];
   int yr, dayOfYear, hh, mm;
   double sss;

   UTCToTimeComps1(ds50UTC, &yr, &dayOfYear, &hh, &mm, &sss);
   sprintf(dtg23, "%02d %03d %02d %02d %010.7f", yr%100, dayOfYear, hh, mm, sss);
   dtg23[23] = 0;

   return dtg23;
}

// CONVERT DAYS SINCE 1950 TO 'YY DDD HH MM SS.SSS'
char* UTCToDtg19aStr(double ds50UTC)
{
   static char dtg19a[20];
   int yr, dayOfYear, hh, mm;
   double sss;

   UTCToTimeComps1(ds50UTC, &yr, &dayOfYear, &hh, &mm, &sss);
   sprintf(dtg19a, "%02d %03d %02d %02d %06.3f", yr%100, dayOfYear, hh, mm, sss);
   dtg19a[19] = 0;

   return dtg19a;
}

// CONVERT DAYS SINCE 1950 TO 'YYYYDDDHHMMSS.SSS'
char* UTCToYYYYDDDHHMMSSsssStr(double ds50UTC)
{
   static char dtg17a[18];
   int yr, dayOfYear, hh, mm;
   double sss;

   UTCToTimeComps1(ds50UTC, &yr, &dayOfYear, &hh, &mm, &sss);
   sprintf(dtg17a, "%04d%03d%02d%02d%06.3f", yr, dayOfYear, hh, mm, sss);
   dtg17a[17] = 0;

   return dtg17a;
}