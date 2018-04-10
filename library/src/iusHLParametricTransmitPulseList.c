//
// Created by Ruijzendaal on 10/04/2018.
//

#define IUSLIBRARY_IMPLEMENTATION
#include <stdlib.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusInput.h>
#include <include/iusHLParametricTransmitPulseList.h>


iuptpl_t iusHLCreateParametricTransmitPulseList
(
    int numTransmitPulses
)
{
    int index;
    IusParametricTransmitPulseList *list = calloc(1, sizeof(IusParametricTransmitPulseList));
    IusParametricTransmitPulse **pulse = calloc((size_t) numTransmitPulses, sizeof(IusParametricTransmitPulse *));
    list->pTransmitPulses =  pulse;
    list->base.count = numTransmitPulses;
    list->base.type = IUS_PARAMETRIC_PULSETYPE;
    for(index=0; index < numTransmitPulses; index++)
    {
        pulse[index] =  (IusParametricTransmitPulse *) iusHLCreateParametricPulse(0.0,0.0,0);
    }
    return list;
}


int iusHLParametricTransmitPulseListSet
(
    IusParametricTransmitPulseList * list,
    float pulseFrequency,
    float pulseAmplitude,
    int pulseCount,
    int pulseIndex
)
{
    IusParametricTransmitPulse *pParametricPulse = NULL;
    if( list == NULL  )
        return IUS_ERR_VALUE;
    if( pulseFrequency < 0.0f )
        return IUS_ERR_VALUE;
    if( pulseCount < 0 )
        return IUS_ERR_VALUE;
    if( pulseIndex >= list->base.count || pulseIndex < 0 )
        return IUS_ERR_VALUE;

    pParametricPulse = list->pTransmitPulses[pulseIndex];
    pParametricPulse->pulseAmplitude = pulseAmplitude;
    pParametricPulse->pulseFrequency = pulseFrequency;
    pParametricPulse->pulseCount = pulseCount;
    return IUS_E_OK;
}

IusParametricTransmitPulse * iusHLParametricTransmitPulseListGet
(
    IusParametricTransmitPulseList *list,
    int pulseIndex
)
{
    if( list == NULL )
        return IUS_FALSE;
    if( pulseIndex >= list->base.count )
        return IUS_FALSE;

    if( list->base.type == IUS_PARAMETRIC_PULSETYPE )
        return list->pTransmitPulses[pulseIndex];
    return NULL;
}


int iusHLParametricTransmitPulseListGetSize
(
    IusParametricTransmitPulseList *list
)
{
    if( list == NULL ) return -1;
    return list->base.count;
}

int iusCompareParametricTransmitPulseList
(
    IusParametricTransmitPulseList *reference,
    IusParametricTransmitPulseList *actual
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
    if( reference->base.type != IUS_PARAMETRIC_PULSETYPE )
        return IUS_FALSE;

    for(index = 0 ; index < actual->base.count ; index++ )
    {
        if( iusCompareParametricPulse( reference->pTransmitPulses[index], actual->pTransmitPulses[index] ) == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}