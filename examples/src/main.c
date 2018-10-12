//
// Created by Ruijzendaal on 07/03/2018.
//
#include "ius.h"

int main
(
  int argc,
  char *argv[]
)
{
  int version =  iusGetVersionMajor();
  if(argc != 2)
    return -1;
  printf("Creating file %s, for version %d\n", argv[0], version);
  iuif_t ifh = iusInputFileCreate(argv[1],10);
  if (ifh == IUIF_INVALID)
  {
    fprintf(stderr,"ERROR: Unable to create file %s",argv[1]);
    return -1;
  }

  // Create Meta data in Node
  iusInputFileClose(ifh);
  iusInputFileDelete(ifh);
  return 0;
}
