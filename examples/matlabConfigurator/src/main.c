#include "iuf.h"
#include <stdio.h>  
#include <unistd.h> 

void printUsage(char **argv)
{
  printf("Usage: for %s:\n", argv[0]);
  printf("%s -b [base] -t [transducer] -d [driving] -n [num_frames] -o [outputfile.m]\n\n", argv[0]);
  printf("where [base] is the name for the basic settings\n");
  printf("where [transducer] is the name for the transducer to include,\n");
  printf("where [driving] is the name of the driving scheme to use, and\n");
  printf("where [num_frames] is the number of frames to record.\n\n");

  printf("[outputfile.m] is the name of the .m script\n\n");
  printf("The labels should refer to .hdf5 files that contain these settings.\n");
}

int main
(
    int argc,
    char *argv[]
)
{
    int version = iufGetVersionMajor();
    int opt;

    while((opt = getopt(argc, argv, ":t:b:d:n:o:")) != -1)  
    {  
        switch(opt)  
        {  
            case 't':  
                printf("transducer: %s\n", optarg);  
                break;
            case 'b':  
                printf("base: %s\n", optarg);  
                break;
            case 'd':  
                printf("driving: %s\n", optarg);  
                break;  
            case 'o':  
                printf("output: %s\n", optarg);  
                break;  
            case ':':  
                printf("option needs a value\n");  
                break;
            case 'n':  
                printf("num frames: %d\n", atoi(optarg)); 
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  

    if (argc != 2)
    {
       printUsage(argv);
       printf("Version=%d\n", version);
    }

    
    return 0;
}
