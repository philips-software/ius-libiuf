//
// Created by Ruijzendaal on 14/03/2018.
//
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <ius.h>
#include <iusUtil.h>
#include <iusError.h>
#include <iusHLInputInstance.h>


// ADT

/** \brief An Ultrasound experiment is identified by a date and a description, also the speed of sound has been determined */
struct IusExperiment
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * pDescription;    /**< Experiment notes */
} ;


iue_t iusHLExperimentCreate
(
float speedOfSound, /**< speed of sound in m/s */
int date,           /**< interger concatenation of year-month-day  */
char *pDescription  /**< Experiment notes */
)
{
    if( speedOfSound < 0.0f ) return IUE_INVALID;
    if( date <= 0 ) return IUE_INVALID;

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
    if( experiment == NULL ) return IUS_ERR_VALUE;
    IusExperiment *pExperiment = experiment;
    free(experiment->pDescription);
    free(experiment);
    return IUS_E_OK;
}

IUS_BOOL iusHLExperimentCompare
(
iue_t reference,
iue_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;

    if( reference->date != actual->date )
        return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->speedOfSound, actual->speedOfSound) == IUS_FALSE )
        return IUS_FALSE;
    return IUS_TRUE;
}

// getters
float iusHLExperimentGetSpeedOfSound
(
    iue_t experiment
)
{
    if( experiment == NULL ) return NAN;
    return experiment->speedOfSound;
}

int iusHLExperimentGetDate
(
    iue_t experiment
)
{
    if( experiment == NULL ) return -1;
    return experiment->date;
}

char * iusHLExperimentGetDescription
(
    iue_t experiment
)
{
    if( experiment == NULL ) return NULL;
    return experiment->pDescription;
}


// old routines
int LF_copyExperimentData
    (
        IusExperiment * pDst,
        IusExperiment * pSrc
    )
{
    // speed of sound in m/s
    // int concatenation of <year><month><day> e.g. 20160123 for 23th Jan 2016
    pDst->speedOfSound = pSrc->speedOfSound;
    pDst->date         = pSrc->date;

    pDst->pDescription  =
        (char *)calloc( strlen( pSrc->pDescription ) + 1, sizeof( char ) );
    if ( pDst->pDescription == NULL )
    {
        return 1;
    }
    strcpy( pDst->pDescription, pSrc->pDescription );

    return 0;
}
