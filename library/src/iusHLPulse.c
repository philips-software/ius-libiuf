//
// Created by Ruijzendaal on 30/03/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusUtil.h>
#include <iusError.h>
#include <iusHLPulse.h>
#include <iusHLPulseImp.h>
#include <iusHLParametricPulse.h>
#include <iusHLNonParametricPulse.h>


iup_t  iusHLPulseCreate
(
    IusPulseType type,
    char *label
)
{
    iup_t transmitPulse = IUP_INVALID;
    if( type == IUS_PARAMETRIC_PULSETYPE ||
        type == IUS_NON_PARAMETRIC_PULSETYPE )
    {
        transmitPulse = calloc(1, sizeof(struct IusPulse));
        transmitPulse->type = type;
        transmitPulse->label = strdup(label);
    }
    return transmitPulse;
}

int iusHLPulseDelete
(
    iup_t pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    free(pulse);
    return IUS_E_OK;
}

int iusHLPulseCompare
(
    iup_t reference,
    iup_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;

    if( reference->type != actual->type )
        return IUS_FALSE;

    if( reference->type == IUS_NON_PARAMETRIC_PULSETYPE  )
    {
        return iusNonParametricPulseCompare((iunpp_t) reference, (iunpp_t) actual);
    }

    if( reference->type == IUS_PARAMETRIC_PULSETYPE  )
    {
        return iusHLParametricPulseCompare((iupp_t) reference, (iupp_t) actual);
    }
    return IUS_FALSE;
}

// getters
IusPulseType iusHLPulseGetType
(
    iup_t pulse
)
{
    if( pulse == NULL ) return  IUS_INVALID_PULSETYPE;
    return pulse->type;
}

char *iusHLPulseGetLabel
(
    iup_t pulse
)
{
    if( pulse == NULL ) return  NULL;
    return pulse->label;
}
