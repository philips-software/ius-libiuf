//
// Created by Ruijzendaal on 05/04/2018.
//



#define IUSLIBRARY_IMPLEMENTATION
#include <stdarg.h>
#include <assert.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusInput.h>
#include <include/iusTypes.h>

#include <include/iusHLTransmitPulseList.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <include/iusHLParametricTransmitPulse.h>


iutpl_t iusHLCreateTransmitPulseList
(
int numTransmitPulses
)
{
    int index;
    IusTransmitPulseList *list = calloc(1, sizeof(IusTransmitPulseList));
    IusTransmitPulse **pulse = calloc((size_t) numTransmitPulses, sizeof(IusTransmitPulseList *));
    list->pTransmitPulses =  pulse;
    list->count = numTransmitPulses;
    for(index=0; index < numTransmitPulses; index++)
    {
        pulse[index] =  NULL;
    }
    return list;
}


int iusHLTransmitPulseListSet
(
    IusTransmitPulseList * list,
    IusTransmitPulse *pulse,
    int index
)
{
    if( list == NULL  )
        return IUS_ERR_VALUE;

    list->pTransmitPulses[index] = pulse;
    return IUS_E_OK;
}

IusTransmitPulse * iusHLTransmitPulseListGet
(
    IusTransmitPulseList *list,
    int index
)
{
    if( list == NULL )
        return IUS_FALSE;
    if( index >= list->count )
        return IUS_FALSE;

    return list->pTransmitPulses[index];
}


int iusHLTransmitPulseListGetSize
(
    IusTransmitPulseList *list
)
{
    if( list == NULL ) return -1;
    return list->count;
}

int iusCompareTransmitPulseList
(
    IusTransmitPulseList *reference,
    IusTransmitPulseList *actual
)
{
    int index;
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;
    if( reference->count != actual->count )
        return IUS_FALSE;

    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusCompareTransmitPulse( reference->pTransmitPulses[index], actual->pTransmitPulses[index] ) == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}