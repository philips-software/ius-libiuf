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
#include <iusHLExperimentImp.h>
#include <include/iusHDF5.h>

// ADT
#define PULSETYPEFMT "%s/speedOfSound"
#define DATEFMT "%s/date"
#define LABELFMT "%s/description"


/** \brief An Ultrasound experiment is identified by a date and a description, also the speed of sound has been determined */
struct IusExperiment
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * pDescription;    /**< Experiment notes */
} ;


iue_t iusHLExperimentCreate
(
	float speedOfSound,		/**< speed of sound in m/s */
	int date,				/**< interger concatenation of year-month-day  */
	char *pDescription		/**< Experiment notes */
)
{
    if ( speedOfSound < 0.0f ) return IUE_INVALID;
    if ( date <= 0 ) return IUE_INVALID;

    IusExperiment *pExperiment = (IusExperiment *) calloc(1, sizeof(IusExperiment));
    pExperiment->speedOfSound = speedOfSound;
    pExperiment->date = date;
    if( pDescription == NULL )
        pExperiment->pDescription = "";
    else
        pExperiment->pDescription = strdup(pDescription);
    return pExperiment;
}

int iusHLExperimentDelete
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
	if (strcmp(reference->pDescription, actual->pDescription) != 0)
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

#if 0
// old routines
int LF_copyExperimentData
(
    iue_t pDst,
    iue_t pSrc
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
#endif

// serialization
int iusHLExperimentSave
(
    iue_t experiment,
    hid_t group_id
)
{
    int status=0;
	const int verbose = 1;

    status |= iusHdf5WriteFloat(group_id, "speedOfSound", &experiment->speedOfSound, 1, verbose);
    status |= iusHdf5WriteInt(group_id, "date", &experiment->date, 1);
    status |= iusHdf5WriteString(group_id, "description", experiment->pDescription, verbose);

    return status;
}

iue_t iusHLExperimentLoad
(
    hid_t handle
)
{
    int status = 0;
    float speedOfSound;
    int date;
    char *pDescription;
    iue_t experiment;

    status |= iusHdf5ReadFloat( handle , "speedOfSound", &(speedOfSound));
    status |= iusHdf5ReadInt( handle,    "date", &(date));
    status |= iusHdf5ReadString( handle, "description", &(pDescription));

    if( status < 0 )
        return NULL;
    experiment = iusHLExperimentCreate(speedOfSound,date,pDescription);
    return experiment;
}

