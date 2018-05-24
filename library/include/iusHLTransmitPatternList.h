//
// Created by Ruijzendaal on 05/04/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPATTERNLIST_H
#define IUSLIBRARY_IUSHLTRANSMITPATTERNLIST_H

#include "iusHLTransmitPattern.h"


// ADT
typedef  struct IusTransmitPatternList IusTransmitPatternList;
typedef  IusTransmitPatternList   * iutpal_t;
#define  IUTPAL_INVALID (iutpal_t) NULL


// operations
iutpal_t iusHLTransmitPatternListCreate
(
int numPulses
);

int iusHLTransmitPatternListDelete
(
IusTransmitPatternList *list
);

int iusHLTransmitPatternListSet
(
    iutpal_t list,
    float time,
    int sourceIndex,
    int pulseIndex,
    int patternIndex
);

iutpa_t iusHLTransmitPatternListGet
(
    IusTransmitPatternList* list,
    int patternIndex
);

int iusHLTransmitPatternListGetSize
(
    iutpal_t list
);

int iusCompareTransmitPatternList
    (
        IusTransmitPatternList *reference,
        IusTransmitPatternList *actual
    );

#endif //IUSLIBRARY_IUSHLTRANSMITPATTERNLIST_H
