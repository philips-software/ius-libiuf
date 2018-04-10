//
// Created by Ruijzendaal on 10/04/2018.
//

#define IUSLIBRARY_IMPLEMENTATION
#include <stdlib.h>
#include <include/iusInput.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <include/iusHLNonParametricTransmitPulseList.h>
#include <include/iusError.h>
#include <include/ius.h>


iunptpl_t iusHLCreateNonParametricTransmitPulseList
(
    int numTransmitPulses,
    int numPulseValues
)
{
    int index;
    IusNonParametricTransmitPulseList *list = calloc(1, sizeof(IusNonParametricTransmitPulseList));
    IusNonParametricTransmitPulse **pulse = calloc((size_t) numTransmitPulses, sizeof(IusNonParametricTransmitPulse *));
    list->pTransmitPulses = pulse;
    list->base.count = numTransmitPulses;
    list->base.type = IUS_NON_PARAMETRIC_PULSETYPE;
    for(index=0; index < numTransmitPulses; index++)
    {
        pulse[index] = iusHLCreateNonParametricPulse(numPulseValues);
    }
    return list;
}


IusNonParametricTransmitPulse * iusHLNonParametricTransmitPulseListGet
(
    IusNonParametricTransmitPulseList *list,
    int pulseIndex
)
{
    if( list == NULL )
        return IUS_FALSE;
    if( pulseIndex >= list->base.count )
        return IUS_FALSE;

    if( list->base.type == IUS_NON_PARAMETRIC_PULSETYPE )
        return list->pTransmitPulses[pulseIndex];
    return NULL;
}

int iusHLNonParametricTransmitPulseListGetSize
(
    IusNonParametricTransmitPulseList *list
)
{
    if( list == NULL ) return -1;
    return list->base.count;
}

int iusHLNonParametricTransmitPulseListSet
(
    iunptpl_t list,
    iunptp_t pulse,
    int pulseIndex
)
{
    if( list == NULL || pulse == NULL ) return IUS_ERR_VALUE;
    if( pulse->base.type != list->base.type ) return IUS_ERR_VALUE;
    if( pulseIndex < 0 ) return IUS_ERR_VALUE;
    if( pulseIndex >= list->base.count ) return IUS_ERR_VALUE;
    list->  pTransmitPulses[pulseIndex] = pulse;
    return IUS_E_OK;
}

int iusCompareNonParametricTransmitPulseList
(
    IusNonParametricTransmitPulseList *reference,
    IusNonParametricTransmitPulseList *actual
)
{
    int index;
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;
    if( reference->base.count != actual->base.count )
        return IUS_FALSE;
    if( reference->base.type != actual->base.type )
        return IUS_FALSE;
    if( reference->base.type != IUS_NON_PARAMETRIC_PULSETYPE )
        return IUS_FALSE;

    for(index = 0 ; index < actual->base.count ; index++ )
    {
        if( iusCompareNonParametricPulse( reference->pTransmitPulses[index], actual->pTransmitPulses[index] ) == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}