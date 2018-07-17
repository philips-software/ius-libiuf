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

// serialization
int iusHLExperimentSave
(
    iue_t experiment,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[64];
	const int verbose = 1;

    // Make dataset for Experiment
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, PULSETYPEFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, path, &experiment->speedOfSound, 1, verbose);
    sprintf(path, DATEFMT, parentPath);
    status |= iusHdf5WriteInt(group_id, path, &experiment->date, 1);
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(group_id, path, experiment->pDescription, verbose);
    status |= H5Gclose(group_id );
    return status;
}

iue_t iusHLExperimentLoad
(
    hid_t handle,
    char *parentPath
)
{
    int status = 0;
    float speedOfSound;
    int date;
    char *pDescription;
    char path[64];
    iue_t experiment;

    sprintf(path, PULSETYPEFMT, parentPath);
    status |= iusHdf5ReadFloat( handle , path, &(speedOfSound));
    sprintf(path, DATEFMT, parentPath);
    status |= iusHdf5ReadInt( handle,    path, &(date));
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5ReadString( handle, path, &(pDescription));

    if( status < 0 )
        return NULL;
    experiment = iusHLExperimentCreate(speedOfSound,date,pDescription);
    return experiment;
}

