//
// Created by Ruijzendaal on 05/04/2018.
//



#define IUSLIBRARY_IMPLEMENTATION
#include <include/iusInput.h>
#include <include/iusHLTransmitPulseList.h>
#include <include/iusError.h>
#include <include/iusTypes.h>


// operations
iutpl_t iusHLCreateTransmitPulseList
(
    int numTransmitPulses,
    IusTransmitPulseType type
)
{
    IusTransmitPulseList *list = calloc(1, sizeof(IusTransmitPulseList));
    if( list == NULL ) return NULL;
    if( type == IUS_NON_PARAMETRIC_PULSETYPE )
    {
        IusParametricTransmitPulse *pulse = calloc(numTransmitPulses, sizeof(IusParametricTransmitPulse));
        list->pTransmitPulse = (IusTransmitPulse *) pulse;
    }

    list->count = numTransmitPulses;
    return list;
}

int iusHLTransmitPulseListSet
(
    IusTransmitPulseList * list,
    IusTransmitPulse * pulse,
    int pulseIndex
)
{
    if( list == NULL || pulse == NULL ) return IUS_ERR_VALUE;
    if( pulseIndex >= list->count ) return IUS_ERR_VALUE;
//    list->pTransmitPulse[pulseIndex] = pulse;
    return IUS_ERR_VALUE;
}


iutp_t iusHLTransmitPulseListGet
(
    iutpl_t list,
    int pulseIndex
)
{
    return NULL;
}


int iusHLTransmitPulseListGetSize
(
    iutpl_t list
)
{
    return -1;
}

int iusCompareTransmitPulseList
(
    iutpl_t reference,
    iutpl_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusCompareTransmitPulse( &reference->pTransmitPulse[index], &actual->pTransmitPulse[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}