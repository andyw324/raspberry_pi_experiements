/*
 gcc-6 stressintPi64.c cpuidc.c -lm -lrt -O3 -march=armv8-a  -o stressintPi64 

*/

 #include <stdio.h>
 #include <stdlib.h>
 #include "cpuidh.h"
 #include <malloc.h>
 #include <string.h>

 unsigned int *xx;
 unsigned int useMemKB = 8;
 unsigned int pattern1[16];
 unsigned int pattern2[16];
 unsigned int usePattern1;
 unsigned int usePattern2;
 unsigned int tempPattern;
 unsigned int printPattern;
 unsigned int mempasses;
 unsigned int isdata[16];
 unsigned int sbdata[16];
 unsigned int errword[16];
 unsigned int sumCheck[8];
 unsigned int outerLoop;
 unsigned int outerLoopc;
 unsigned int calcResult =  { 0x00000000 }; 
 unsigned int passWords;


 int     testlog       = 0;
 int     RrunSecs = 1;
 int     TrunSecs;
 int     testPasses = 1;
 int     n1;
 int     errorTest;
 int     errors = 0;
 int     errord;
 int     errorp;

 char    version[30] = "RPi 64";
 char    logfile[80] = "";
 char    iparam[100] = "";
 char    testmsg[30][256];
 char    errormsg[100] = "                    No Errors Detected ";
 char    patternSB[20];
 char    patternIs[20];
 char    cpu_config[1000];
 
 double n1Count;
 double timeCount;
 double millionBytes;
 double results[30][10];
 
 FILE    *outfile;

//  ************** PROTOPTYPES *****************

 void checkData();
 void writeData();
 void errorcheck();
 void readData();
 

 int main(int argc, char *argv[])
 {         
    int     i, j, p, g;
    int     param1;
    int     endit = 1;
    int     noConfig = 1;
    char    val[20];
       
    for (i=1; i<11; i=i+1)
    {
       if (argc > i)
       {
          strcat(iparam, argv[i]); 
          switch (toupper(argv[i][0]))
          {
               case 'K':
                param1 = 0;
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%d", &param1);
                   sprintf(val, " %d, ", param1); 
                   strcat(iparam, val); 
                   if (param1 > 0) useMemKB = param1;
                }
                break;
                
                case 'S':
                param1 = 0;
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%d", &param1);
                   sprintf(val, " %d, ", param1); 
                   strcat(iparam, val); 
                   if (param1 > 0) RrunSecs = param1;
                }
                break;

                case 'L':
                param1 = 0;
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%d", &param1);
                   i=i+1;
                   if (param1 > 0) testlog = param1;
                   if (testlog > 99) testlog = 0;
                }
                break;

                case  'P':
                endit = 0;
                break;

                case  'N':
                noConfig = 1;
                break;
         }
       }
    }
    
    if (RrunSecs > 10)
    {
        float tp = (float)RrunSecs / 10.0;
        testPasses = (int)tp;
        RrunSecs = (int)((float)RrunSecs / (float)testPasses);
    }
    TrunSecs = RrunSecs * testPasses;
   
    pattern1[0]  = 0x00000000;
    pattern1[1]  = 0xFFFFFFFF;
    pattern1[2]  = 0xA5A5A5A5;
    pattern1[3]  = 0x55555555;
    pattern1[4]  = 0x33333333;
    pattern1[5]  = 0xF0F0F0F0;
    pattern1[6]  = 0x00000000;
    pattern1[7]  = 0xFFFFFFFF;
    pattern1[8]  = 0xA5A5A5A5;
    pattern1[9]  = 0x55555555;
    pattern1[10] = 0x33333333;
    pattern1[11] = 0xF0F0F0F0;

    pattern2[0]  = 0x00000000;
    pattern2[1]  = 0xFFFFFFFF;
    pattern2[2]  = 0x5A5A5A5A;
    pattern2[3]  = 0xAAAAAAAA;
    pattern2[4]  = 0xCCCCCCCC;
    pattern2[5]  = 0x0F0F0F0F;
    pattern2[6]  = 0xFFFFFFFF;
    pattern2[7]  = 0x00000000;
    pattern2[8]  = 0x5A5A5A5A;
    pattern2[9]  = 0xAAAAAAAA;
    pattern2[10] = 0xCCCCCCCC;
    pattern2[11] = 0x0F0F0F0F;

    sprintf (logfile, "logInt%d.txt", testlog);
    local_time();
    outfile = fopen(logfile,"a+");
    if (outfile == NULL)
    {
        printf (" Cannot open results file \n\n");
        printf(" Press Enter\n");
        g  = getchar();
        exit (0);
    }

    getDetails();
   
     
    printf("\n");
    fprintf (outfile, "\n"); 
    fprintf (outfile, " ##############################################\n\n");
    printf ("\n");
    printf (" ##############################################\n\n");

    if (!noConfig)
    {
        printf ("\nFrom File /proc/cpuinfo\n");
        printf("%s\n", configdata[0]);
        printf ("\nFrom File /proc/version\n");
        printf("%s\n", configdata[1]);
        printf("%s\n", cpu_config);
    }
    
    local_time();

    printf("  Integer Stress Test %s %s\n", version, timeday);
    fprintf(outfile, "  Integer Stress Test %s %s\n", version, timeday);

    printf("   %d KBytes Cache or RAM Space, %d Seconds Per Test, 12 Tests\n\n", useMemKB, TrunSecs);
    fprintf(outfile,"   %d KBytes Cache or RAM Space, %d Seconds Per Test, 12 Tests\n\n", useMemKB, TrunSecs);
    
    fflush(outfile);
    fflush(stdout);

    xx = (unsigned int *)malloc((unsigned int)useMemKB*1024+256);
    if (xx  == NULL)
    {
        printf(" ERROR WILL EXIT\n");
        printf(" Press Enter\n");
        g  = getchar();
        exit(1);
    }

    fprintf(outfile, " Write/Read\n");
    printf(" Write/Read\n");
    fflush(outfile);
    mempasses = useMemKB * 8;   // 128 bytes or 32 words per pass
    errorTest = 0;

    for (j=6; j<12; j++)
    {
        printPattern = pattern1[j];
        usePattern1 = pattern2[j];
        usePattern2 = pattern1[j];
        millionBytes = (double) useMemKB * 1.024 / 1000;
        results[j][0] = 0;
        n1Count = 0;
        timeCount = 0;
       
        for (p=0; p<testPasses; p++)
        {
            n1 = 0;
            start_time();
            do
            {
                tempPattern = usePattern1;
                usePattern1 = usePattern2;
                usePattern2 = tempPattern;
                xx[0]   = usePattern1;
                xx[1]   = usePattern1;
                xx[2]   = usePattern1;
                xx[3]   = usePattern1;
                xx[4]   = usePattern2;
                xx[5]   = usePattern2;
                xx[6]   = usePattern2;
                xx[7]   = usePattern2;
                errorTest = errorTest + 1;
                writeData();
        // Test for error reports one word
        //        if (errorTest == 1 && p == 0 && j == 6) xx[1] = 99;                                
                checkData();
                n1 = n1 + 1;
                end_time();
            }
            while (secs < (double)RrunSecs && errors == 0);
            n1Count = n1Count + (double)n1;
            timeCount = timeCount + secs;
            printf(" Log%2d Pass %3d Test%2d of 6, Pattern %8.8X, %4.1f Secs,  %6.0f MB/S\n",
                       testlog, p+1, j-5, printPattern, secs, (millionBytes * 2 * (double)n1 / secs));
        }
        results[j][0] = millionBytes * 2 * n1Count / timeCount;

        sprintf(patternSB, "%8.8X", usePattern1);
        if (errors == 0)
        {
             sprintf(testmsg[j], " Pattern %8.8X Result OK %10.0f passes", printPattern, n1Count);
        }
        else
        {
            sprintf(testmsg[j], " Pattern %s  ERROR", patternSB);
        }
        fprintf(outfile, " %2d %7.0f MB/sec %s\n", j-5, results[j][0], testmsg[j]);
        if (errors > 0)
        {
            fprintf(outfile, " %d 4 byte words checked and %d errors found\n", useMemKB*256, errors);
            if (errors < 12)
            {
                fprintf(outfile, " %d errors\n", errord);
            }
            else
            {
                fprintf(outfile, " First 12 errors\n");
            } 
            for (i=0; i<errord; i++)
            {
                fprintf(outfile, " Pattern %8.8X Was %8.8X Word %d\n", sbdata[i], isdata[i], errword[i]);
            }
            sprintf(errormsg, "                    ERRORS - See %s logfile ", logfile);
        }
        fflush(outfile);
    }
    printf("\n%s\n\n", errormsg);


   // ################  Part 2

    sprintf(errormsg, "                    No Errors Detected ");
    fprintf(outfile, " Read\n");
    printf(" Read\n");
    fflush(outfile);

    for (j=0; j<6; j++) // 0 to 6
    {

        printPattern = pattern1[j];
        usePattern1 = pattern1[j];   
        usePattern2 = pattern2[j];   
        
        millionBytes = (double) useMemKB * 1.024 / 1000;


        xx[0]   = usePattern1;
        xx[1]   = usePattern1;
        xx[2]   = usePattern1;
        xx[3]   = usePattern1;
        xx[4]   = usePattern2;
        xx[5]   = usePattern2;
        xx[6]   = usePattern2;
        xx[7]   = usePattern2;


        mempasses = useMemKB * 8;
        writeData();
        checkData();

        if (errors > 0)
        {
            fprintf(outfile, "    ERROR found checking data after generation - Test %d\n", j+1);
            fprintf(outfile, " %d 4 byte words checked and %d errors found\n", useMemKB*256, errors);
            if (errors < 12)
            {
                fprintf(outfile, " %d errors\n", errord);
            }
            else
            {
                fprintf(outfile, " First 12 errors\n");
            } 
            for (i=0; i<errord; i++)
            {
                fprintf(outfile, " Pattern %8.8X Was %8.8X Word %d\n", sbdata[i], isdata[i], errword[i]);
            }
            sprintf(errormsg, "                    ERRORS - See %s logfile ", logfile);
            
            fflush(outfile);
        }

        results[j][0] = 0;

        passWords = 32;
        mempasses = useMemKB * 256 / passWords;

        outerLoop = 1;
        start_time();
        readData();
        end_time();

        outerLoop = 100;
        if (secs > 0.001) outerLoop = (unsigned int)(0.1/secs);
        if (outerLoop < 1) outerLoop = 1;
        n1Count = 0;
        timeCount = 0;
        outerLoopc = outerLoop;
        
        for (p=0; p<testPasses; p++)
        {
            n1 = 0;
            start_time();
            do
            {
                outerLoop = outerLoopc;
                readData();
                n1 = n1 + 1;
                end_time();
            }
            while (secs < (double)RrunSecs);
            n1Count = n1Count + (double)n1;
            timeCount = timeCount + secs;
            outerLoop = outerLoopc;
            printf(" Log%2d Pass %3d Test%2d of 6, Pattern %8.8X, %4.1f Secs,  %6.0f MB/S\n",
                       testlog, p+1, j+1, printPattern, secs, (millionBytes * (double)outerLoop * (double)n1 / secs));
        }
        results[j][0] = millionBytes * (double)outerLoop * n1Count / timeCount;
        double icount = (38 * (double)mempasses + 11) * (double)outerLoop;
        sprintf(patternSB, "%8.8X", usePattern1);
        sprintf(patternIs, "%8.8X",  calcResult);
        sprintf(testmsg[j], " Pattern %8.8X Result OK %10.0f passes", printPattern, n1Count * (double)outerLoop);

        if (strcmp(patternSB, patternIs) != 0)
        {
            sprintf(testmsg[j], " Pattern %s %c ERROR Was %s", patternSB, 9, patternIs);
            errorp = 1;
        }
        fprintf(outfile, " %2d %7.0f MB/sec %s\n", j+1, results[j][0], testmsg[j]);
        fflush(outfile);        

        checkData();

        if (errors == 0)
        {
            if (errorp > 0)
            {
                fprintf(outfile, " %d 4 byte words checked but no errors found\n", useMemKB*256);
            }
        }
        else
        {
            if (errorp == 0)
            {
                fprintf(outfile, " No Errors in Reliability/Speed Test FAILED ON END COMPARE\n");
            }
            fprintf(outfile, " %d 4 byte words checked and %d errors found\n", useMemKB*256, errors);
            if (errors < 12)
            {
                fprintf(outfile, " %d errors\n", errord);
            }
            else
            {
                fprintf(outfile, " First 12 errors\n");
            } 
            for (i=0; i<errord; i++)
            {
                fprintf(outfile, " Pattern %8.8X Was %8.8X Word %d\n", sbdata[i], isdata[i], errword[i]);
            }
        }
        fflush(outfile);
        if (errors > 0 || errorp > 0)
        {
            sprintf(errormsg, "                    ERRORS - See %s logfile ", logfile);
        }
    }

    fprintf(outfile, "\n");
    printf("\n%s\n\n", errormsg);


    free(xx);
    local_time();
    fprintf(outfile, "\n                   End at %s\n", timeday);
    printf("\n                    End at %s\n", timeday);    


    if (!noConfig)
    {
        fprintf (outfile, " SYSTEM INFORMATION\n\nFrom File /proc/cpuinfo\n");
        fprintf (outfile, "%s \n", configdata[0]);
        fprintf (outfile, "\nFrom File /proc/version\n");
        fprintf (outfile, "%s \n", configdata[1]);
    }
    fprintf (outfile, "\n");

    fflush(outfile);

    char moredata[1024];
    if (!endit)
    {
        printf("\n Type additional information to include in %s - Press Enter\n", logfile);
        if (fgets (moredata, sizeof(moredata), stdin) != NULL)
        fprintf (outfile, " Additional information - %s\n", moredata);
    }
    fflush(outfile);
    fflush(stdout);

    fclose (outfile);
    return 0;
 }


 void writeData()
 {
     int i, j;
     unsigned int ecx = xx[0];
     unsigned int ecy = xx[0];
     unsigned int edx = xx[4];
     unsigned int edy = xx[4];
     unsigned int eax = 8;
     
     for (i=0; i<mempasses; i++)
     {
         xx[eax]    = ecx;
         xx[eax+1]  = ecx;
         xx[eax+2]  = ecx;
         xx[eax+3]  = ecx;
         xx[eax+4]  = edx;
         xx[eax+5]  = edx;
         xx[eax+6]  = edx;
         xx[eax+7]  = edx;
         xx[eax+8]  = ecx;
         xx[eax+9]  = ecx;
         xx[eax+10] = ecx;
         xx[eax+11] = ecx;
         xx[eax+12] = edx;
         xx[eax+13] = edx;
         xx[eax+14] = edx;
         xx[eax+15] = edx;
         xx[eax+16] = ecx;
         xx[eax+17] = ecx;
         xx[eax+18] = ecx;
         xx[eax+19] = ecx;
         xx[eax+20] = edx;
         xx[eax+21] = edx;
         xx[eax+22] = edx;
         xx[eax+23] = edx;
         xx[eax+24] = ecx;
         xx[eax+25] = ecx;
         xx[eax+26] = ecx;
         xx[eax+27] = ecx;
         xx[eax+28] = edx;
         xx[eax+29] = edx;
         xx[eax+30] = edx;
         xx[eax+31] = edx;
         eax = eax + 32;
     }
 }

void checkData()
{
    int i;
    unsigned int m;

    sumCheck[0] = usePattern1;
    sumCheck[1] = usePattern2;

    mempasses = useMemKB * 8; // 1024 / 128;            
    errorcheck();
    
    errors = 0;
    errord = 0;

    if (sumCheck[0] != usePattern1 ||
        sumCheck[1] != usePattern1 || 
        sumCheck[2] != usePattern2 ||
        sumCheck[3] != usePattern2 ||  errorp == 1)
    {
        for (m=0; m<useMemKB*128; m=m+8)
        {
           if (sumCheck[0] != usePattern1 ||
               sumCheck[1] != usePattern1 || errorp == 1)
           {    
               for (i=0; i<4; i++)
               {
                  if (xx[m+i] != usePattern1)
                  {
                      errors = errors + 1;
                      if (errors < 13)
                      {
                          isdata[errord] = xx[m+i];
                          sbdata[errord] = usePattern1;
                          errword[errord] = m+i;
                          errord = errord + 1;
                      }
                  }
               }
           }
           if (sumCheck[2] != usePattern2 ||
               sumCheck[3] != usePattern2 || errorp == 1) 
           {
               for (i=4; i<8; i++)
               {
                  if (xx[m+i] != usePattern2)
                  {
                      errors = errors + 1;
                      if (errors < 13)
                      {
                          isdata[errord] = xx[m+i];
                          sbdata[errord] = usePattern2;
                          errword[errord] = m+i+4;
                          errord = errord + 1;
                      }
                  }
               }
           }
        }
    }
}

void errorcheck()
{
    int i;
    unsigned int eax = 0;
    unsigned int ecx = sumCheck[0];
    unsigned int edx = sumCheck[0];
    unsigned int edi = sumCheck[1];
    unsigned int esi = sumCheck[1];
    for (i=0; i<mempasses; i++)
    {        
         ecx = ecx | xx[eax]    |  xx[eax+1] | xx[eax+2]  | xx[eax+3];
         edi = edi | xx[eax+4]  |  xx[eax+5] | xx[eax+6]  | xx[eax+7];
         ecx = ecx | xx[eax+8]  |  xx[eax+9] | xx[eax+10] | xx[eax+11];
         edi = edi | xx[eax+12] | xx[eax+13] | xx[eax+14] | xx[eax+15];
         ecx = ecx | xx[eax+16] | xx[eax+17] | xx[eax+18] | xx[eax+19];
         edi = edi | xx[eax+20] | xx[eax+21] | xx[eax+22] | xx[eax+23];
         ecx = ecx | xx[eax+24] | xx[eax+25] | xx[eax+26] | xx[eax+27];
         edi = edi | xx[eax+28] | xx[eax+29] | xx[eax+30] | xx[eax+31];
         edx = edx & xx[eax]    &  xx[eax+1] & xx[eax+2]  & xx[eax+3];
         esi = esi & xx[eax+4]  &  xx[eax+5] & xx[eax+6]  & xx[eax+7];
         edx = edx & xx[eax+8]  &  xx[eax+9] & xx[eax+10] & xx[eax+11];
         esi = esi & xx[eax+12] & xx[eax+13] & xx[eax+14] & xx[eax+15];
         edx = edx & xx[eax+16] & xx[eax+17] & xx[eax+18] & xx[eax+19];
         esi = esi & xx[eax+20] & xx[eax+21] & xx[eax+22] & xx[eax+23];
         edx = edx & xx[eax+24] & xx[eax+25] & xx[eax+26] & xx[eax+27];
         esi = esi & xx[eax+28] & xx[eax+29] & xx[eax+30] & xx[eax+31];
         eax = eax + 32;         
    }
    sumCheck[0] = ecx;
    sumCheck[1] = edx;
    sumCheck[2] = edi;
    sumCheck[3] = esi;   
}

void readData()
{
    int i, j;
    unsigned int eax;
    unsigned int ecx = xx[0];
    unsigned int edx = xx[0];
    unsigned int edi = xx[0];
    unsigned int esi = xx[0];

    for (j=0; j<outerLoop; j++)
    {
        eax = 0;        
        for (i=0; i<mempasses; i++)
        {
            ecx = ecx - xx[eax];
            edx = edx - xx[eax+1];       
            edi = edi - xx[eax+2];       
            esi = esi - xx[eax+3];
            ecx = ecx - xx[eax+4];
            edx = edx - xx[eax+5];       
            edi = edi - xx[eax+6];       
            esi = esi - xx[eax+7];
            ecx = ecx + xx[eax+8];
            edx = edx + xx[eax+9];       
            edi = edi + xx[eax+10];       
            esi = esi + xx[eax+11];
            ecx = ecx + xx[eax+12];
            edx = edx + xx[eax+13];       
            edi = edi + xx[eax+14];       
            esi = esi + xx[eax+15];                   
            ecx = ecx - xx[eax+16];
            edx = edx - xx[eax+17];       
            edi = edi - xx[eax+18];       
            esi = esi - xx[eax+19];
            ecx = ecx - xx[eax+20];
            edx = edx - xx[eax+21];       
            edi = edi - xx[eax+22];       
            esi = esi - xx[eax+23];
            ecx = ecx + xx[eax+24];
            edx = edx + xx[eax+25];       
            edi = edi + xx[eax+26];       
            esi = esi + xx[eax+27];
            ecx = ecx + xx[eax+28];
            edx = edx + xx[eax+29];       
            edi = edi + xx[eax+30];       
            esi = esi + xx[eax+31];
            eax = eax + passWords;                   
        }        
    }
//    printf("%8.8X %8.8X %8.8X %8.8X SB %8.8X eax %d\n", ecx, edx, esi, edi, xx[0], eax); 

    ecx = ecx - edx;
    ecx = ecx + edi;
    ecx = ecx - esi;
    ecx = ecx + edi;
    calcResult = ecx;
    xx[0] = ecx;
}

