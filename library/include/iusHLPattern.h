
// Created by nlv09165 on 13/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERN_H
#define IUSLIBRARY_IUSHLPATTERN_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct IusPattern IusPattern;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct IusPattern IusPattern;
typedef IusPattern *iup_t;
#define  IUP_INVALID (iup_t) NULL

iup_t iusHLPatternCreate
(
    char* pLabel,
    float timeInFrame
);

int iusHLPatternDelete
(
    iup_t iusPattern
);


// operations
int iusHLPatternCompare
(
    iup_t reference,
    iup_t actual
);

// Getters
int iusHLPatternGetIntParam
(
    iup_t iusPattern
);

float iusHLPatternGetFloatParam
(
    iup_t iusPattern
);

// Setters
int iusHLPatternSetIntParam
(
    iup_t iusPattern,
    int intParam
);

int iusHLPatternSetFloatParam
(
    iup_t iusPattern,
    float floatParam
);


#endif //IUSLIBRARY_IUSHLPATTERN_H
