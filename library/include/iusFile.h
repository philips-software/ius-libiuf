
// Created by nlv09165 on 31/08/2018.
#ifndef IUSLIBRARY_IUSFILE_H
#define IUSLIBRARY_IUSFILE_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct IusFile IusFile;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct IusFile IusFile;
typedef IusFile *iuf_t;
#define  IUF_INVALID (iuf_t) NULL

iuf_t iusFileCreate
(
    int intParam,
    float floatParam
);

int iusFileDelete
(
    iuf_t iusFile
);


// operations
int iusFileCompare
(
    iuf_t reference,
    iuf_t actual
);

// Getters
int iusFileGetIntParam
(
    iuf_t iusFile
);

float iusFileGetFloatParam
(
    iuf_t iusFile
);

// Setters
int iusFileSetIntParam
(
    iuf_t iusFile,
    int intParam
);

int iusFileSetFloatParam
(
    iuf_t iusFile,
    float floatParam
);


#endif //IUSLIBRARY_IUSFILE_H
