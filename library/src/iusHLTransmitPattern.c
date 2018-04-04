//
// Created by Ruijzendaal on 04/04/2018.
//

#define IUSLIBRARY_IMPLEMENTATION

#include <include/iusInput.h>
#include <iusHLTransmitPattern.h>
#include <stdlib.h>
#include <include/ius.h>
#include <math.h>
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
    int pulseIndex,
    int patternIndex
)
{
    if( pulseIndex < 0 || patternIndex < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || patternIndex >= list->count ) return IUS_ERR_VALUE;
    list->pTransmitPattern[patternIndex].time = time;
    list->pTransmitPattern[patternIndex].index = pulseIndex;
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

int iusHLTransmitPatternSet
(
    IusTransmitPattern *pattern,
    float time,
    int pulseIndex
)
{
    int status = IUS_E_OK;
    if( pulseIndex < 0 ) return IUS_ERR_VALUE;
    pattern->index = pulseIndex;
    pattern->time = time;
    return status;
}


float iusHLTransmitPatternGetTime
(
    IusTransmitPattern *pattern
)
{
    if( pattern == NULL ) return NAN;
    return pattern->time;
}


int iusHLTransmitPatternGetIndex
(
    iutpa_t pattern
)
{
    if( pattern == NULL ) return -1;
    return pattern->index;
}


int iusCompareTransmitPattern
(
    IusTransmitPattern *reference,
    IusTransmitPattern *actual
)
{
    if( reference->index != actual->index ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->time, actual->time) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}
