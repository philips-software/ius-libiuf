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


iuf_t iusHLOpenFile
(
    const char *filename
);

iuf_t iusHLCreateFile
(
    const char *filename
);

int iusHLFileSave
(
    IusInputFileInstance *fileHandle
);

int iusHLCloseFile
(
    iuf_t fileHandle
);

iuh_t iusHLFileGetHeader
(
    iuf_t fileHandle
);

int iusHLFileSetHeader
(
    iuf_t fileHandle,
    iuh_t header
);

#endif //IUSLIBRARY_IUSHLINPUTFILE_H
