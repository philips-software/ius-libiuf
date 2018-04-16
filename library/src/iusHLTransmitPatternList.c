//
// Created by Ruijzendaal on 05/04/2018.
//


#define IUSLIBRARY_IMPLEMENTATION

#include <stdlib.h>
#include <math.h>
#include <include/iusInput.h>
#include <include/iusHLTransmitPatternList.h>
#include <include/iusHLTransmitPattern.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>

IusTransmitPatternList *iusHLCreateTransmitPatternList
(
    int numTransmitPulses
)
{
    IusTransmitPatternList *pPatternList = calloc(1, sizeof(IusTransmitPatternList));
    if(pPatternList!=NULL)
    {
        pPatternList->count = numTransmitPulses;
        pPatternList->pTransmitPattern = (IusTransmitPattern *) calloc(numTransmitPulses, sizeof(IusTransmitPattern));
        if( pPatternList->pTransmitPattern == NULL )
        {
            free(pPatternList);
            pPatternList = NULL;
        }

    }
    return pPatternList;
}

int iusHLDeleteTransmitPatternList
(
    IusTransmitPatternList* list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


int iusHLTransmitPatternListGetSize
(
    IusTransmitPatternList* list
)
{
    return list->count;
}

IusTransmitPattern* iusHLTransmitPatternListGet
(
    IusTransmitPatternList* list,
    int patternIndex
)
{
    if( patternIndex < 0 ) return NULL;
    if( list == NULL || patternIndex >= list->count ) return NULL;
    return &list->pTransmitPattern[patternIndex];
}

int iusHLTransmitPatternListSet
(
    IusTransmitPatternList* list,
    float time,
    int sourceIndex,
    int pulseIndex,
    int patternIndex
)
{
    if( sourceIndex < 0 || pulseIndex < 0 || patternIndex < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || patternIndex >= list->count ) return IUS_ERR_VALUE;
    list->pTransmitPattern[patternIndex].time = time;
    list->pTransmitPattern[patternIndex].sourceIndex = sourceIndex;
    list->pTransmitPattern[patternIndex].pulseIndex = pulseIndex;
    return IUS_E_OK;
}


int iusCompareTransmitPatternList
    (
        IusTransmitPatternList *reference,
        IusTransmitPatternList *actual
    )
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusCompareTransmitPattern( &reference->pTransmitPattern[index], &actual->pTransmitPattern[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}
