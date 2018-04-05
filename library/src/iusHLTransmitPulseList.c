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
    int numTransmitPulses
)
{
    return NULL;
}

int iusHLTransmitPulseListSet
(
    iutpl_t list
)
{
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
    return IUS_FALSE;
}