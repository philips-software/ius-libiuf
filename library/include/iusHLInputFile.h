//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H

#include <iusTypes.h>

#ifndef IUSLIBRARY_IMPLEMENTATION
struct IusInputFileInstance;
typedef struct IusInputFileInstance IusInputFileInstance;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusInputFileInstance * iuf_t;

iuf_t iusHLCreateFile(const char *filename, iuh_t header);
int iusSaveHeader(iuf_t fileHandle,iuh_t header);
int iusHLCloseFile(iuf_t fileHandle);
iuf_t iusOpenFile(const char *filename);
iuh_t iusGetHeader(iuf_t fileHandle);

#endif //IUSLIBRARY_IUSHLINPUTFILE_H
