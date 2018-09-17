//
// Created by Ruijzendaal on 08/03/2018.
//
#ifndef IUS_H
#define IUS_H

#include <iusTypes.h>
#include <iusError.h>

#define  IUS_E_OK  0
#define  IUH_INVALID NULL
#define  IUS_INPUT_TYPE "IUSINPUTFILETYPE_V3"

int iusGetVersionMajor
(
  void
);

IUS_BOOL fileExists
(
  const char *fname
);

#endif //IUS_H
