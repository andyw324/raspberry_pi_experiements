/* 

gcc-6  drivespeedLong.c cpuidc.c -lm -lrt -O3 -march=armv8-a -o DriveSpeed64Long

*/
 #define _GNU_SOURCE

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include "cpuidh.h"
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <dirent.h>
 #include <sys/statvfs.h>
 #include <malloc.h>
 
FILE    *outfile;

// ************* GLOBAL VARIABLES **********
 
char   version[30] = "64-Bit Version 1.2";
// char   version[30] = "32-Bit Version 1.2";

int  *dataIn;
int  *dataOut;
int dataSize = 1048576;
int smallSize = 1024;
int largeFile = 1;
int fileMB = 8;
int fileKB = 2;
int fileMBcache = 8;
int fileMBbus = 8;
int fileMBrand;
char testFile[PATH_MAX];
char filePath[PATH_MAX] = "";
int handle;
int useCache;
int blockSize;
int randBlocks;
int randPos;
int g;

int writeFile(int use, int dsize);
int readFile(int use, int dsize);


int main(int argc, char *argv[])
{
    int i, f, k, m, p, s;
    int f1, f2, totalf;
    double mbps;
    double msecs;
    double totalMB;
    double freeMB;
    double runningTime;
    double lastSecs;
    double c;
    int files = 5;
    int sizes = 3;
    int param1;

    for (i=1; i<5; i=i+2)
    {
       if (argc > i)
       {
          switch (toupper(argv[i][0]))
          {
                case 'M':
                param1 = 0;
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%d", &param1);
                   if (param1 > 0)
                   {
                      fileMB = param1;
                   }
                }
                break;

                case 'F':
                if (argc > i+1)
                {
                   sscanf(argv[i+1],"%s", filePath);
                   strcat(filePath, "/");
                }
                break;

          }
       }
    }

    outfile = fopen("IOSpeed.txt","a+");
    if (outfile == NULL)
    {
        printf (" Cannot open results file \n\n");
        printf(" Press Enter\n");
        g  = getchar();
        exit (0);
    }
    printf("\n");

//    getDetails();

    for (i=1; i<10; i++)
    {
//        printf("%s\n", configdata[i]);
    }
    printf("\n");

   dataIn  = (int*)malloc(dataSize);
   dataOut = (int*)malloc(dataSize);

    fprintf (outfile, " ###############################################################\n\n");                     
    for (i=1; i<10; i++)
    {
//        fprintf(outfile, "%s \n", configdata[i]);
    }
    fprintf (outfile, "\n");
 
    printf(" ##########################################\n"); 
    fprintf (outfile, " ###############################################################\n");                     

    printf("\n");
    fprintf (outfile, "\n");


    char thispath[PATH_MAX];

    if (strcmp(filePath, "") != 0)
    { 
        printf (" Selected File Path: \n %s\n", filePath);
        fprintf (outfile, " Selected File Path: \n %s\n", filePath);
        strcpy(thispath, filePath);
    }
    else
    {
        if (getcwd(thispath, sizeof thispath) == NULL)
        {
            printf(" Current Directory Path not available\n");
            fprintf(outfile, " Current Directory Path not available\n");
        }
        else
        {
            printf (" Current Directory Path: \n %s\n", thispath);
            fprintf (outfile, " Current Directory Path: \n %s\n", thispath);
        }
    }

    struct statvfs driveSpace;
    if (statvfs(thispath, &driveSpace))
    {
        printf(" Free Disk Space not available - Exit\n");
        fprintf(outfile, " Free Disk Space not available - Exit\n");
        fclose(outfile);
        free(dataIn);
        free(dataOut);
        printf(" Press Enter\n");
        g  = getchar();
        exit (0);
    }
    else
    {  
      totalMB = (double)driveSpace.f_frsize * (double)driveSpace.f_blocks / 1048576.0;
      freeMB  = (double)driveSpace.f_frsize * (double)driveSpace.f_bfree  / 1048576.0;
      printf (" Total MB %7.0f, Free MB %7.0f, Used MB %7.0f\n", totalMB, freeMB, totalMB - freeMB);
      fprintf (outfile, " Total MB %7.0f, Free MB %7.0f, Used MB %7.0f\n", totalMB, freeMB, totalMB - freeMB);
    }

    if (freeMB < (40.0 + 5 * fileMB))
    {
        printf (" Exit - Not Enough Free Space\n\n");
        fprintf (outfile, " Exit - Not Enough Free Space\n\n");
        fclose(outfile);
        free(dataIn);
        free(dataOut);
        printf(" Press Enter\n");
        g  = getchar();
        exit (0);
    }
    if (freeMB < (40.0 + 5 * fileMB * 2))
    {
        printf(" Not Enough Free Disk Space - Only 5 x %d MB attempted\n", fileMB);
        fprintf(outfile, " Not Enough Free Disk Space - Only 5 x %d MB attempted\n", fileMB);
        sizes = 1;
    }
    if (freeMB < (40.0 + 5 * fileMB * 4))
    {
        printf(" Not Enough Free Disk Space - Only 5 x %d MB attempted\n", fileMB * 2);
        fprintf(outfile, " Not Enough Free Disk Space - Only 5 x %d MB attempted\n", fileMB * 2);
        sizes = 2;
    }
    local_time();

    printf("\n Linux Storage Speed Test %s, %s\n", version, timeday);      
    printf("                Copyright (C) Roy Longbottom 2012\n\n");    
    fprintf(outfile, "\n Linux Storage Speed Test %s, %s\n", version, timeday);      
    fprintf(outfile, "                Copyright (C) Roy Longbottom 2012\n\n");    
    fflush(outfile);
    
    for (i=0; i<262144; i++)
    {
        dataIn[i]  = 0;
        dataOut[i] = i;
    }

    // Large Files
    useCache = 0;  
    for (s=0; s<sizes; s++)
    {
        if (s > 0) fileMB = fileMB * 2;
        runningTime = 0.0;
        printf          ("  %4d MB File         1          2          3          4          5\n", fileMB);
        fprintf(outfile, "  %4d MB File         1          2          3          4          5\n", fileMB);
        printf          (" Writing MB/sec");
        fprintf(outfile, " Writing MB/sec");
    
        // Write
        for (f=0; f<5; f++)
        {
            sprintf(testFile, "%szzztestz%d", filePath, f); 

            if (!writeFile(fileMB, dataSize))
            {
                for (f=0; f<5; f++)
                {
                   sprintf(testFile, "%szzztestz%d", filePath, f);        
                   remove(testFile);
                }  
                free(dataIn);
                free(dataOut);
                exit (0);
            }
            mbps = (double)fileMB * (double)dataSize/ 1000000.0 / secs;
            printf          (" %10.2f", mbps);
            fprintf(outfile, " %10.2f", mbps);
            fflush(stdout);
            runningTime = runningTime + secs;
            lastSecs = secs;
        }
        printf("\n"); 
        fprintf(outfile, "\n"); 
    
        printf          (" Reading MB/sec");
        fprintf(outfile, " Reading MB/sec");
    
        //  Read
        for (f=0; f<5; f++)
        {
            sprintf(testFile, "%szzztestz%d", filePath, f);

            if (!readFile(fileMB, dataSize))  
            {
                for (f=0; f<5; f++)
                {
                  sprintf(testFile, "%szzztestz%d", filePath, f);        
                  remove(testFile);
                }  
                free(dataIn);
                free(dataOut);
                 exit (0);
            }

            mbps = (double)fileMB * (double)dataSize/ 1000000.0 / secs;
            printf          (" %10.2f", mbps);
            fprintf(outfile, " %10.2f", mbps);
            fflush(stdout);
            runningTime = runningTime + secs;
        }
        printf("\n\n"); 
        fprintf(outfile, "\n\n"); 
    
        for (f=0; f<5; f++)
        {
            sprintf(testFile, "%szzztestz%d", filePath, f);        
            remove(testFile);
        }
        if (s + 1 < sizes)
        {
            if (runningTime > 60)
            {
                 printf(" Running Time Too Long At %d Seconds - No More File Sizes\n", (int)runningTime); 
                 fprintf(outfile, " Running Time Too Long At %d Seconds - No More File Sizes\n", (int)runningTime); 
                 sizes = 1;
            }
        }
    }

    // Cached
    useCache = 1;
    fprintf(outfile, " ---------------------------------------------------------------------\n");     
    printf          (" 8 MB Cached File      1          2          3          4          5\n");
    fprintf(outfile, " 8 MB Cached File      1          2          3          4          5\n");
    printf          (" Writing MB/sec");
    fprintf(outfile, " Writing MB/sec");

    // Write
    for (f=0; f<5; f++)
    {
        sprintf(testFile, "%szzztestz%d", filePath, f); 

        if (!writeFile(fileMBcache, dataSize))
        {
            for (f=0; f<5; f++)
            {
               sprintf(testFile, "%szzztestz%d", filePath, f);        
               remove(testFile);
            }  
            free(dataIn);
            free(dataOut);
            exit (0);
        }

        mbps = (double)fileMBcache * (double)dataSize/ 1000000.0 / secs;
        printf          (" %10.2f", mbps);
        fprintf(outfile, " %10.2f", mbps);
        fflush(stdout);
    }
    printf("\n"); 
    fprintf(outfile, "\n"); 

    printf          (" Reading MB/sec");
    fprintf(outfile, " Reading MB/sec");

    //  Read
    for (f=0; f<5; f++)
    {
        sprintf(testFile, "%szzztestz%d", filePath, f);

        if (!readFile(fileMBcache, dataSize))  
        {
            for (f=0; f<5; f++)
            {
               sprintf(testFile, "%szzztestz%d", filePath, f);        
               remove(testFile);
            }  
            free(dataIn);
            free(dataOut);
            exit (0);
        }

        mbps = (double)fileMBcache * (double)dataSize/ 1000000.0 / secs;
        printf          (" %10.2f", mbps);
        fprintf(outfile, " %10.2f", mbps);
        fflush(stdout);
    }
    printf("\n\n"); 
    fprintf(outfile, "\n\n"); 

    for (f=0; f<5; f++)
    {
        sprintf(testFile, "%szzztestz%d", filePath, f);        
        remove(testFile);
    }

    // Bus Speed
    useCache = 0; 

    // Write
    fprintf(outfile, " ---------------------------------------------------------------------\n");     
    printf          (" Bus Speed Block KB     64        128        256        512       1024\n");
    fprintf(outfile, " Bus Speed Block KB     64        128        256        512       1024\n");
    printf          (" Reading MB/sec");
    fprintf(outfile, " Reading MB/sec");

    sprintf(testFile, "%szzztestz99", filePath); 
    if (!writeFile(fileMBbus, dataSize))
    {
         remove("testFile");
         exit (0); 
    }
    
    // Read
    blockSize = 64 * 1024;

    handle = open(testFile, O_RDONLY | O_DIRECT | O_SYNC);
    if (handle == -1)
    {
        printf (" Cannot open data file for reading\n\n");
        fprintf (outfile, " Cannot open data file for reading\n\n");
        fclose(outfile);
        remove("testFile");
        free(dataIn);
        free(dataOut);
        printf(" Press Enter\n");
        g  = getchar();
        exit (0);
    }

    for (p=0; p<5; p++)
    {
        c = 0.0;

        start_time();
        do
        {
            for (k=0; k<25; k++)
            {
                lseek(handle, 0, SEEK_SET);
                if (read(handle, dataIn, blockSize) == -1)               
                {
                    printf (" Error reading file\n\n");
                    fprintf (outfile, " Error reading file\n\n");
                    fclose(outfile);
                    close(handle);
                    remove("testFile");
                    free(dataIn);
                    free(dataOut);
                    printf(" Press Enter\n");
                    g  = getchar();
                    exit (0);
                }           
            }
            end_time();
            c = c + 25.0;
        }
        while (secs < 1.0);
        mbps = (double)blockSize * c / 1000000.0 / secs;
        printf          (" %10.2f", mbps);
        fprintf(outfile, " %10.2f", mbps);
        fflush(stdout);
            
        blockSize = blockSize * 2;
    }
    close(handle);
    printf("\n\n"); 
    fprintf(outfile, "\n\n"); 
    remove(testFile);

    // Random 
    fileMBrand = 4 * fileMB;
    if (fileMBrand > 128) fileMBrand = 128;
    randBlocks = fileMBrand * 1024;
    blockSize = 1024;
    printf (" Writing %d MB Data - WAIT %6.1f seconds?\n\n", fileMBrand, lastSecs * 4);

    // Write
    sprintf(testFile, "%szzzrandz", filePath); 
    if (!writeFile(fileMBrand, dataSize))
    {
        remove("testFile");
        free(dataIn);
        free(dataOut);
        exit (0);
    }

    fprintf(outfile, " ---------------------------------------------------------------------\n");     
    printf          (" 1 KB Blocks File MB >    2      4      8     16     32     64    128\n");
    fprintf(outfile, " 1 KB Blocks File MB >    2      4      8     16     32     64    128\n");
    printf          (" Random Read  msecs ");
    fprintf(outfile, " Random Read  msecs ");

    
    srand(123);
    handle = open(testFile, O_RDWR | O_DIRECT | O_SYNC);
    if (handle == -1)
    {
        printf (" Cannot open data file for reading and writing\n\n");
        fprintf (outfile, " Cannot open data file for reading and  writing\n\n");
        fclose(outfile);
        remove("testFile");
        free(dataIn);
        free(dataOut);
        printf(" Press Enter\n");
        g  = getchar();
        exit (0);
    }

    // Random Read

    for (p=2; p<fileMBrand+1; p=p*2)
    {

        c = 0.0;
        start_time();
        do
        {
            for (k=0; k<25; k++)
            {
                randPos = (int)((double)p * 1024.0 * ((double)(rand()) / (double)2147483648.0));
                if (randPos == randBlocks) randPos = randPos - 1;
                lseek(handle, randPos * 1024, SEEK_SET);
                posix_fadvise(handle, 0,0,POSIX_FADV_DONTNEED);
                if (read(handle, dataIn, blockSize) == -1)               
                {
                    printf (" Error reading file\n\n");
                    fprintf (outfile, " Error reading file\n\n");
                    fclose(outfile);
                    close(handle);
                    remove("testFile");
                    free(dataIn);
                    free(dataOut);
                    printf(" Press Enter\n");
                    g  = getchar();
                    exit (0);
                }           
            }
            end_time();
            c = c + 25.0;
        }
        while (secs < 1.0);

        msecs = 1000.0 * secs / c;
        printf          ("%7.2f", msecs);
        fprintf(outfile, "%7.2f", msecs);
        fflush(stdout);

    }
    printf("\n"); 
    fprintf(outfile, "\n"); 
    printf          (" Random Write msecs ");
    fprintf(outfile, " Random Write msecs ");

    // Random Write

    for (p=2; p<fileMBrand+1; p=p*2)
    {
        c = 0.0;
        start_time();
        do
        {
            for (k=0; k<25; k++)
            {
                randPos = (int)((double)p * 1024.0 * ((double)(rand()) / (double)2147483648.0));
                if (randPos == randBlocks) randPos = randPos - 1;
                lseek(handle, randPos * 1024, SEEK_SET);
                posix_fadvise(handle, 0,0,POSIX_FADV_DONTNEED);
                if (write(handle, dataOut, blockSize) == -1)               
                {
                    printf (" Error reading file\n\n");
                    fprintf (outfile, " Error reading file\n\n");
                    fclose(outfile);
                    close(handle);
                    remove("testFile");
                    free(dataIn);
                    free(dataOut);
                    printf(" Press Enter\n");
                    g  = getchar();
                    exit (0);
                }           
            }
            end_time();
            c = c + 25.0;
        }
        while (secs < 1.0);

        msecs = 1000.0 * secs / c;
        printf          ("%7.2f", msecs);
        fprintf(outfile, "%7.2f", msecs);
        fflush(stdout);
    }
    printf("\n\n"); 
    fprintf(outfile, "\n\n"); 
    remove(testFile);

    // Small Files
    largeFile = 0;
    useCache = 0;
    f1 = 1000;
    f2 = 1500;
    totalf = f2 - f1;
    fprintf(outfile, " ---------------------------------------------------------------------\n");     
    fprintf(outfile, " 500 Files   Write             Read             Delete\n");  
    fprintf(outfile, " File KB     MB/sec  ms/File   MB/sec  ms/File  Seconds\n");
    printf(          " 500 Files            Write             Read             Delete\n");  
    printf(          " File KB   Progress   MB/sec  ms/File   MB/sec  ms/File  Seconds\n");

    for (fileKB=2; fileKB<(fileMB*1024)/totalf; fileKB=fileKB*2)
    {
        if (fileKB > 2) sleep(5);
        printf(          "%8d ", fileKB);
        fprintf(outfile, "%8d  ", fileKB);

        // Write
        m = 0;
        start_time();
        for (f=f1; f<f2; f++)
        {
            sprintf(testFile, "%sztestz%d", filePath, f); 
            if (!writeFile(1, smallSize * fileKB))
            {
                for (f=f1; f<f2; f++)
                {
                    sprintf(testFile, "%sztestz%d", filePath, f); 
                    remove(testFile);
                }
                free(dataIn);
                free(dataOut);
                exit (0);
            }
            m = m + 1;
            if (m > 48)
            {
                m = 0;
                printf("-");
                fflush(stdout);
            }
        }

        end_time();
        mbps = (double)totalf * (double)fileKB * (double)smallSize / 1000000.0 / secs;
        msecs = secs * 1000.0 / (double)totalf;
        fprintf(outfile, "%9.2f%9.2f", mbps, msecs);
        printf(          "%9.2f%9.2f", mbps, msecs);
        fflush(stdout);

        // Read
        start_time();
        for (f=f1; f<f2; f++)
        {
            sprintf(testFile, "%sztestz%d", filePath, f); 

            if (!readFile(1, smallSize * fileKB))
            {
                for (f=f1; f<f2; f++)
                {
                    sprintf(testFile, "%sztestz%d", filePath, f); 
                    remove(testFile);
                }
                free(dataIn);
                free(dataOut);
                exit (0);
            }
        }
        end_time();
        mbps = (double)totalf * (double)fileKB * (double)smallSize / 1000000.0 / secs;
        msecs = secs * 1000.0 / (double)totalf;
        fprintf(outfile, "%9.2f%9.2f", mbps, msecs);
        printf(          "%9.2f%9.2f", mbps, msecs);
        fflush(stdout);
 
       
        start_time();
        for (f=f1; f<f2; f++)
        {
            sprintf(testFile, "%sztestz%d", filePath, f); 
            remove(testFile);
        }
        end_time();

        fprintf(outfile, "%9.3f\n", secs);
        printf(          "%9.3f Wait 5s\n", secs);
        fflush(outfile);
        fflush(stdout);
    }
    local_time();
    fprintf(outfile, "\n              End of test %s", timeday);        
    fprintf(outfile,"\n");
    free(dataIn);
    free(dataOut);
    
    fflush(outfile);
    fclose(outfile);
    printf("\n Press Enter\n");
    g  = getchar();

    return 1;
    
} // main



int writeFile(int use, int dsize)
{
    int  p;
    
    if (largeFile) start_time();

    if (useCache)
    {
          handle = open(testFile, O_WRONLY | O_CREAT | O_TRUNC,
                                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }
    else
    {
          handle = open(testFile, O_WRONLY | O_CREAT | O_TRUNC  | O_DIRECT | O_SYNC ,
                                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }
    if (handle == -1)
    {
        printf (" Cannot open data file for writing\n\n");
        fprintf (outfile, " Cannot open data file for writing\n\n");
        fclose(outfile);
        printf(" Press Enter\n");
        g  = getchar();
        return 0;
    }

    for (p=0; p<use; p++)
    {
       if (write(handle, dataOut, dsize) != dsize )
       {
            printf (" Error writing file\n\n");
            fprintf (outfile, " Error writing file\n\n");
            fclose(outfile);
            close(handle);
            printf(" Press Enter\n");
            g  = getchar();
            return 0;
        }
    }
    close(handle);
    if (largeFile) end_time();
    return 1;
}

int readFile(int use, int dsize)
{
    int p;

    if (largeFile) start_time();
    if (useCache)
    {
          handle = open(testFile, O_RDONLY);
    }
    else
    {
          handle = open(testFile, O_RDONLY | O_DIRECT | O_SYNC);
    }

    if (handle == -1)
    {
        printf (" Cannot open data file for reading\n\n");
        fprintf (outfile, " Cannot open data file for reading\n\n");
        fclose(outfile);
        printf(" Press Enter\n");
        g  = getchar();
        return 0;
    }

    for (p=0; p<use; p++)
    {
        if (read(handle, dataIn, dsize) == -1)
        {
            printf (" Error reading file\n\n");
            fprintf (outfile, " Error reading file\n\n");
            fclose(outfile);
            close(handle);
            printf(" Press Enter\n");
            g  = getchar();
            return 0;
        }           
 
    }
    close(handle);
    if (largeFile) end_time();
    return 1;
}
   
