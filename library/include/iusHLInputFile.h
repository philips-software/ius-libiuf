//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H



// ADT
struct IusInputFile;
typedef struct IusInputFile IusInputFile;
typedef  IusInputFile * iuf_t;


iuf_t iusHLInputFileOpen
(
const char *filename
);

iuf_t iusHLInputFileCreate
(
const char *filename
);

int iusHLInputFileSave
(
iuf_t fileHandle
);

int iusHLInputFileClose
(
iuf_t fileHandle
);

iuii_t iusHLInputFileGetInstance
(
iuf_t fileHandle
);

int iusHLInputFileSetInstance
(
iuf_t fileHandle,
iuii_t instance
);

#endif //IUSLIBRARY_IUSHLINPUTFILE_H
