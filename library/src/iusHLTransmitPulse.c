//
// Created by Ruijzendaal on 30/03/2018.
//



#define IUSLIBRARY_IMPLEMENTATION
#include <include/iusInput.h>
#include <include/iusHLTransmitPulse.h>
#include <math.h>
#include <stdlib.h>
#include <include/ius.h>
#include <include/iusUtil.h>
#include <include/iusError.h>
#include <include/iusHLParametricTransmitPulse.h>
#include <include/iusHLNonParametricTransmitPulse.h>


int iusCompareTransmitPulse
(
    IusTransmitPulse *reference,
    IusTransmitPulse *actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;

    if( reference->type != actual->type )
        return IUS_FALSE;

    if( reference->type == IUS_NON_PARAMETRIC_PULSETYPE  )
    {
        return iusCompareNonParametricPulse((IusNonParametricTransmitPulse *)reference,(IusNonParametricTransmitPulse *)actual);
    }

    if( reference->type == IUS_PARAMETRIC_PULSETYPE  )
    {
        return iusCompareParametricPulse((IusParametricTransmitPulse *)reference,(IusParametricTransmitPulse *)actual);
    }
    return IUS_FALSE;
}

