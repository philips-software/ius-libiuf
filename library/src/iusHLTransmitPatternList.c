//
// Created by Ruijzendaal on 05/04/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLTransmitPattern.h>
#include <iusHLTransmitPatternList.h>

// ADT
struct IusTransmitPatternList
{
    int count;
    iutpa_t *     pTransmitPattern;       /**< array (time, index) of length numTransmitPulses */
} ;

iutpal_t iusHLTransmitPatternListCreate
(
    int numPulses
)
{
    iutpal_t pPatternList = calloc(1, sizeof(IusTransmitPatternList));
    if(pPatternList!=NULL)
    {
        pPatternList->count = numPulses;
        pPatternList->pTransmitPattern = (iutpa_t *) calloc(numPulses, sizeof(iutpa_t));
        if( pPatternList->pTransmitPattern == NULL )
        {
            free(pPatternList);
            pPatternList = NULL;
        }
    }
    return pPatternList;
}

int iusHLTransmitPatternListDelete
(
    iutpal_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


int iusCompareTransmitPatternList
(
    iutpal_t reference,
    iutpal_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if(iusHLTransmitPatternCompare(reference->pTransmitPattern[index], actual->pTransmitPattern[index])
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHLTransmitPatternListGetSize
(
    iutpal_t list
)
{
    return list->count;
}

iutpa_t iusHLTransmitPatternListGet
(
    iutpal_t list,
    int patternIndex
)
{
    if( patternIndex < 0 ) return NULL;
    if( list == NULL || patternIndex >= list->count ) return NULL;
    return list->pTransmitPattern[patternIndex];
}

int iusHLTransmitPatternListSet
(
    iutpal_t list,
    float time,
    int sourceIndex,
    int pulseIndex,
    int patternIndex
)
{
    if( sourceIndex < 0 || pulseIndex < 0 || patternIndex < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || patternIndex >= list->count ) return IUS_ERR_VALUE;
    iusHLTransmitPatternSet(list->pTransmitPattern[patternIndex], time, sourceIndex,pulseIndex);
    return IUS_E_OK;
}

