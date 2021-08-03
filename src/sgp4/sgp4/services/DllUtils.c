#include "DllUtils.h"
#include <math.h>

// Get input parameters from the command line
void GetCommandLineArgs(int argc, char* argv[], const char* exeName, const char* appName, CommandLineArgs* cla)
{
   int i;

   // nullify all string fields
   cla->bConst[0]  = 0;
   cla->logFile[0] = 0;
   cla->libPath[0] = 0;
   cla->sgp4LicPath[0] = 0;


   // This program is getting input parameters from the command line.
   // Follow the instruction to pass parameters in the right order.
   if (argc >= 3 )
   {
      strcpy(cla->inFile,   argv[1]);
		strcpy(cla->outFile , argv[2]);
      printf("Input file     = %s\n", cla->inFile);
      printf("Output file    = %s\n", cla->outFile);
      for(i = 3; i < argc; i++)
      {
         if (strncmp(argv[i], "-I", 2) == 0) // optional -IlibPath provided
         {
            strcpy(cla->libPath, &argv[i][2]);
         }
         else if (strncmp(argv[i], "-D", 2) == 0) // optional -Dlogfile provided
         {
            strcpy(cla->logFile, &argv[i][2]);
            printf("Debug log file = %s\n", cla->logFile);
         }
         else if (strncmp(argv[i], "-S", 2) == 0) // optional -Ssgp4Lic provided
         {
            strcpy(cla->sgp4LicPath, &argv[i][2]);
            printf("Sgp4 license key path = %s\n", cla->sgp4LicPath);
         }
         else if (strlen(argv[i]) == 7)// optional Earth constants provided
         {

            strcpy(cla->bConst,  argv[i]);
            printf("Earth Const    = %s\n", cla->bConst);
         }
      }
	}
	else 
   {
      printf("Error in number of parameters passed. Please see the usage.\n\n\n");
      printf("Usage     : %s inFile outFile [EarthConstant] [-Ilibpath] [-Dlogfile]\n\n", exeName);
      printf("inFile    : File contains %s related input data\n", appName);
      printf("outFile   : Name of the output file\n");
      printf("EarthConst: Optional gravitational constants ('WGS-72', 'WGS-84', 'EGM-96' ...)\n");
      printf("-IlibPath : Optional path \"libpath\" to the Astrodynamic Standards library.\n");
      printf("-DlogFile : Optional \"logFile\" to enable writing debug data to the specified log file.\n");
      printf("-Ssgp4Lic : Optional path \"sgp4Lic\" to the SGP4_Open_License.txt file.\n");
      if (strncmp("ROTAS", appName, 5) == 0)
         printf("-LD       : Optional option to print residuals in old format (less decimal places).\n");
      if (strncmp("COMBO", appName, 5) == 0)
         printf("-M        : Optional option to clear out SP ojbects to prevent runing out of virtual memory problem.\n");
      if (strncmp("ROTAS_IDS", appName, 9) == 0)
      {
         printf("Additional VDS options : [-P<period>] [-Z<stepSize>] [-N]\n");
         printf("P : period (in minutes) to be categorized as very deep space (VDS)\n");
         printf("Z : step size (in minutes) for VDS\n");
         printf("N : apply nutation computation for VDS\n");
      }
      exit(1);
   }

   // if not specify sgp4Lic, assign the licPath to it
   // "SGP4_Open_License.txt" is in the same folder as the Sgp4Prop.dll
   if (cla->sgp4LicPath[0] == 0 && cla->libPath[0] != 0)
      strcpy(cla->sgp4LicPath, cla->libPath);

   printf("Library Path   = %s\n\n", cla->libPath);
}   


// Explicitly load the dll
void* LoadLib(const char* dllName)
{
   void* hLib = NULL;

   #ifdef _WIN32
      hLib = LoadLibraryA(dllName);
   #else
      hLib = dlopen(dllName, RTLD_LAZY);
   #endif

   if (hLib == NULL)
   {
      printf("Unable to load the library \"%s\".\n", dllName);
      #ifdef _WIN32
         printf("GetLastError() returns code # %d\n", GetLastError());
      #else
         printf("dlerror() returns string: %s\n", dlerror());
      #endif
      
      exit(1);
   }
   else
   {  
      // Uncomment this if want to show what DLL is being loaded
      printf("%s loaded successfully.\n", dllName);
   }

   return hLib;
}


//Return the function pointer to the DLL function. Check to see if
//it is located successfully.
void* GetFnPtr(void* hLib, char* fName)
{
   void* fnctPtr;

#ifdef _WIN32
   fnctPtr = GetProcAddress(hLib, fName);
#else
   // Unix-family operating systems create all-lowecase function name
   // DHN 18Feb14 - Unix-family OSs' Astro Stds dlls now have the same function names as Windows PC platform's (Pascal notation)
   // fnctPtr = dlsym(hLib, ToStrLower(fName));
   fnctPtr = dlsym(hLib, fName);
#endif

   if (fnctPtr == NULL) 
   {
      printf("Error! Unable to locate function: \"%s\" in the Dll.\n", fName);
      exit(1);
   }

   return fnctPtr;
}


//Convert a string to lowercase
char* ToStrLower(char* inStr)
{
   int i;
   static char strLower[50]; //Dll function name in lowercase not to exceed 50 chars

   for(i = 0; inStr[i] != 0 && i < 49; i++)
   {
      strLower[i] = tolower(inStr[i]);
   }
   strLower[i] = 0; //nullify the string

   return strLower;
}



// Free the specified library
void FreeLib(void* hLib, const char* dllName)
{
   int ok;

// Unload the library before leaving the program
#ifdef _WIN32
   ok = FreeLibrary(hLib);
   if (ok != 0) // In Windows, if the function succeeds, the return value is nonzero
#else
   ok = dlclose(hLib);
   if (ok == 0) // Unix returns 0 on success
#endif
      printf("%s freed successfully.\n", dllName);
   else
   {
      printf("%s freed unsuccessfully.\n", dllName);
      //exit(1);
   }
}

// Open file base on specified mode and check for error
FILE* FileOpen(char* fileName, char* mode)
{
   FILE* fp;

   Rtrim(fileName);
   
   if ((fp = fopen(fileName, mode)) == NULL )
   {
      printf("Error: File '%s' failed to open.", fileName);
      exit(1);
   }

   return fp;
}

// Remove trailing blanks from a fileName
void Rtrim(char* fileName)
{
   int len;
   int i;

   len = (int)strlen(fileName);
   for(i = len - 1; i >= 0 && fileName[i] == ' '; i--)
   {
      fileName[i] = 0;
   }
}


// Copy one job from a batch input file to a scratch file
char* GetOneJob(FILE* inputFilePtr, int* isEndAll)
{
	static char outputFileName[256];	/* scratch file name */
   static char uniqueFileName[256];	/* scratch file name */
	FILE* outputFilePtr;				/* output file written to */

	char line[1001];	/* buffer to read line of input file into */
	int lineNumber;	/* index of line being read in */
	char* s;				/* pointer to input line buffer */
	size_t len;			/* number of characters contained in line read in */
	size_t i;			/* loop index - used to loop thru char in a line */
	int blankline;    /* 0 indicates line is not blank; 1 indicates blank line */
   int n;

   outputFileName[0] = 0;
   *isEndAll = 0;

	/*
	 * Check input parameters:
	 *   - input file must be open already,
	 *     AND it must remain open during subsequent calls
	 */
	if (inputFilePtr == NULL)
	{
		printf("ERROR: Input file should be open.\n");
	}
	else
	{	
      // Try to create a unique file name for 10 times
      for(n = 0; n < 10; n++)
      {
         // DHN 17Aug11 - Auto generate a unique file name each time

         // Use different function on different platform
#ifdef _WIN32
         if(tmpnam(uniqueFileName) == NULL)
         {
            outputFileName[0] = 0;
            return outputFileName;
         }
         // Create temporary unique file name in current folder ".\uniqueFileName__ScratchFile__"
         sprintf(outputFileName, "%s%s", uniqueFileName, "__ScratchFile__");

#else
         // Create temporary unique file name in current folder "__ScratchFile__uniqueFileName"
         strncpy(outputFileName, "__ScratchFile__XXXXXX", 21);
         if (mkstemp(outputFileName) == -1)
         {
            outputFileName[0] = 0;
            return outputFileName;
         }
#endif
          
		   outputFilePtr = fopen(outputFileName, "w");

         // Successfully opened the unique file name
         if(outputFilePtr != NULL)
            break;
      }
      

		if (outputFilePtr == NULL) 
		{
			printf("ERROR: Cannot open temporary file: '%s' \n", outputFileName);
			outputFileName[0] = 0;
		}
		else
		{	/*
			 * Read a line from the input file.
			 * If hit eof or ENDALL, there are no lines left to write to the scratch file - bailout;
			 * otherwise, write line to the scratch file - bailing out after reading & writing ENDOFJOB.
			 */
			for (lineNumber = 1; 1; lineNumber++)
			{
				s = fgets(line, 1000, inputFilePtr);

				if ((s == NULL) || (strncmp(line, "ENDALL", 6) == 0))
				{
               // signify end of file or end all has reached
               *isEndAll = 1;

               // DHN 14Mar11 - End of file reached and file contains no input data
               if(lineNumber == 1)
               {
                  fclose(outputFilePtr);
                  remove(outputFileName);
                  outputFileName[0] = 0;
                  return outputFileName;
               }
					break;
				}
				len = strlen(line);
				if (len > 0 && line[len-1] != '\n')
				{
					printf("ERROR: Input file contains line > 999 characters in length. \n");
				}

				blankline = 1; /* assume a blank line */
				for (i=0; i < len; i++)	/* check each ch on the line*/
				{
					if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') /* if any ch on the line is not space or tab or newline then line not blank */ 
					{
						blankline = 0;
						break;
					}
				}
				if (!blankline)
				{
					fputs(line, outputFilePtr);
				}
				if (strncmp(line, "ENDOFJOB", 8) == 0)
				{
					break;
				}
			}

		   fclose (outputFilePtr);
		}
	}
	/*
	 * Returns either the name of the output file 
	 * or NULL when hit eof of input file
	 */
	return outputFileName;
}  


// Print Export Controlled warning
void PrintWarning(FILE* fp, const char* softwareName)
{
   fprintf(fp, "%s\n", "**********************************************************");
   fprintf(fp, "%s\n", "*                                                        *");
   fprintf(fp, "%s\n", "*                      W A R N I N G                     *");
   fprintf(fp, "%s\n", "*  THIS SOFTWARE CONTAINS TECHNICAL DATA WHOSE EXPORT IS *");
   fprintf(fp, "%s\n", "*  RESTRICTED BY THE ARMS EXPORT CONTROL ACT (TITLE 22,  *");
   fprintf(fp, "%s\n", "*  USC, SEC 2751 ) OR EXECUTIVE ORDER 12470. VIOLATORS   *");
   fprintf(fp, "%s\n", "*  OF EXPORT LAWS ARE SUBJECT TO SEVERE CRIMINAL         *");
   fprintf(fp, "%s\n", "*  PENALTIES.                                            *");
   fprintf(fp, "%s\n", "*                 D I S T R I B U T I O N                *");
   fprintf(fp, "%s\n", "*  DISTRIBUTION IS AUTHORIZED TO US GOVERNMENT AGENCIES  *");
   fprintf(fp, "%s\n", "*  AND THEIR CONTRACTORS FOR OFFICIAL USE ON A NEED TO   *");
   fprintf(fp, "%s\n", "*  KNOW BASIS ONLY. ALL REQUESTS FOR THIS SOFTWARE SHALL *");
   fprintf(fp, "%s\n", "*  BE REFERRED TO AFSPC/A9.  NO SOFTWARE CODE, MANUAL,   *");
   fprintf(fp, "%s\n", "*  OR MEDIA CONTAINING ANY REPRESENTATION OF THE UNITED  *");
   fprintf(fp, "%s\n", "*  STATES AIR FORCE (USAF), HQ AIR FORCE SPACE COMMAND   *");
   fprintf(fp, "%s\n", "*  (AFSPC) SPACE ANALYSIS CENTER (ASAC) [AFSPC/A9]       *");
   fprintf(fp, "*                   %10s                           *\n", softwareName);
   fprintf(fp, "%s\n", "*  CAPABILITY MAY BE ASSIGNED, COPIED, OR TRANSFERRED TO *");
   fprintf(fp, "%s\n", "*  ANY NON-AUTHORIZED PERSON, CONTRACTOR, OR GOVERNMENT  *");
   fprintf(fp, "%s\n", "*  AGENCY WITHOUT THE EXPRESSED WRITTEN CONSENT OF       *");
   fprintf(fp, "%s\n", "*               USAF, HQ AFSPC/A9.                       *");
   fprintf(fp, "%s\n", "**********************************************************\n\n");
}


