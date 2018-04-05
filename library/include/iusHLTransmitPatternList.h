//
// Created by Ruijzendaal on 05/04/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPATTERNLIST_H
#define IUSLIBRARY_IUSHLTRANSMITPATTERNLIST_H

#ifndef IUSLIBRARY_IMPLEMENTATION
typedef  struct IusTransmitPatternList IusTransmitPatternList;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusTransmitPatternList   * iutpal_t;
#define  IUTPAL_INVALID (iutpal_t) NULL


// operations
iutpal_t iusHLCreateTransmitPatternList
(
    int numTransmitPulses
);

int iusHLTransmitPatternListSet
(
    iutpal_t list,
    float time,
    int pulseIndex,
    int patterniIndex
);

IusTransmitPattern* iusHLTransmitPatternListGet
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
