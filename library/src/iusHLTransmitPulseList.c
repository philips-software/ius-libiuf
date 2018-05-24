//
// Created by Ruijzendaal on 05/04/2018.
//



#include <stdarg.h>
#include <assert.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>

#include <include/iusHLTransmitPulseList.h>
#include <include/iusHLNonParametricPulse.h>
#include <include/iusHLParametricPulse.h>


// ADT
struct IusTransmitPulseList
{
    int count;
    iup_t *pTransmitPulses;
} ;

iutpl_t iusHLCreateTransmitPulseList
(
    int numTransmitPulses
)
{
    int index;
    int size1 = sizeof(IusTransmitPulseList);
    int size2 = sizeof(struct IusTransmitPulseList);
    IusTransmitPulseList *list = calloc(1, sizeof(IusTransmitPulseList));
    iup_t *pulses = calloc((size_t) numTransmitPulses, sizeof(iup_t));
    list->pTransmitPulses =  pulses;
    list->count = numTransmitPulses;
    for(index=0; index < numTransmitPulses; index++)
    {
        pulses[index] =  NULL;
    }
    return list;
}


int iusHLTransmitPulseListSet
(
    iutpl_t list,
    iup_t pulse,
    int index
)
{
    if( list == NULL  )
        return IUS_ERR_VALUE;

    list->pTransmitPulses[index] = pulse;
    return IUS_E_OK;
}

iup_t iusHLTransmitPulseListGet
(
    iutpl_t list,
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
    iutpl_t list
)
{
    if( list == NULL ) return -1;
    return list->count;
}

int iusCompareTransmitPulseList
(
    iutpl_t reference,
    iutpl_t actual
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
        if(iusHLComparePulse( reference->pTransmitPulses[index], actual->pTransmitPulses[index] ) == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}