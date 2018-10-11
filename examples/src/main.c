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
  if(argc != 1)
    return -1;
  printf("Creating file %s, for version %d\n", argv[0], version);
  return 0;
}
