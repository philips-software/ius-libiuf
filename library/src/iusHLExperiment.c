//
// Created by Ruijzendaal on 14/03/2018.
//
#define IUSLIBRARY_IMPLEMENTATION
#include <iusInput.h>
#include <include/iusHLInput.h>
#include <stdlib.h>
#include <iusInput.h>
#include <memory.h>
#include <include/ius.h>
#include <include/iusInput.h>
#include <include/iusUtil.h>
#include <math.h>
#include <include/iusUtil.h>



IUS_BOOL iusCompareExperiment(iue_t reference, iue_t actual) {
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;

    if( reference->date != actual->date ) return IUS_FALSE;
    if( reference->speedOfSound != actual->speedOfSound)  return IUS_FALSE;
    return IUS_TRUE;
}

iue_t iusHLCreateExperiment
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    char *pDescription  /**< Experiment notes */
)
{
    IusExperiment *pExperiment = (IusExperiment *) calloc(1, sizeof(IusExperiment));
    pExperiment->speedOfSound = speedOfSound;
    pExperiment->date = date;
    if( pDescription == NULL )
        pExperiment->pDescription = "";
    else
        pExperiment->pDescription = strdup(pDescription);
    return pExperiment;
}


int iusHLDeleteExperiment
(
    iue_t experiment
)
{
    IusExperiment *pExperiment = experiment;
    free(experiment->pDescription);
    free(experiment);
    return IUS_E_OK;
}

float iusHLExperimentGetSpeedOfSound
(
    iue_t experiment
)
{
    return experiment->speedOfSound;
}

int iusHLExperimentGetDate
(
    iue_t experiment
)
{
    return experiment->date;
}

char * iusHLExperimentGetDescription
(
    iue_t experiment
)
{
    return experiment->pDescription;
}

