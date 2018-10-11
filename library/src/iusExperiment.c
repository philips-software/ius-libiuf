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
#include <iusInputFileStructure.h>
#include <iusExperimentPrivate.h>
#include <include/iusHDF5.h>

// ADT
//#define PULSETYPEFMT "%s/speedOfSound"
//#define LABELFMT "%s/description"

/** \brief An Ultrasound experiment is identified by a date and a description, also the speed of sound has been determined */
struct IusExperiment
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * pDescription;    /**< Experiment notes */
} ;

iue_t iusExperimentCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    const char *pDescription  /**< Experiment notes */
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

int iusExperimentDelete
(
    iue_t experiment
)
{
    if( experiment == NULL ) return IUS_ERR_VALUE;
    free(experiment->pDescription);
    free(experiment);
    return IUS_E_OK;
}

IUS_BOOL iusExperimentCompare
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
float iusExperimentGetSpeedOfSound
(
    iue_t experiment
)
{
    if( experiment == NULL ) return NAN;
    return experiment->speedOfSound;
}

int iusExperimentGetDate
(
    iue_t experiment
)
{
    if( experiment == NULL ) return -1;
    return experiment->date;
}

char * iusExperimentGetDescription
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
int iusExperimentSave
(
    iue_t experiment,
    hid_t handle
)
{
    int status=0;

	hid_t experiment_id;
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_EXPERIMENT, 0, NULL); // todo centralize the path "Sources"
	if (status != 0) // the group does not exist yet
	{
		experiment_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_EXPERIMENT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		experiment_id = H5Gopen(handle, IUS_INPUTFILE_PATH_EXPERIMENT, H5P_DEFAULT);
	}
	if (experiment_id > 0) status = 0;
    status |= iusHdf5WriteFloat(experiment_id, IUS_INPUTFILE_PATH_EXPERIMENT_SPEEDOFSOUND, &experiment->speedOfSound, 1);
    status |= iusHdf5WriteInt(experiment_id, IUS_INPUTFILE_PATH_EXPERIMENT_DATE, &experiment->date, 1);
    status |= iusHdf5WriteString(experiment_id, IUS_INPUTFILE_PATH_EXPERIMENT_DESCRIPTION, experiment->pDescription);

	H5Gclose(experiment_id);
    return status;
}

iue_t iusExperimentLoad
(
    hid_t handle
)
{
    int status = 0;
    float speedOfSound;
    int date;
    char description[IUS_MAX_HDF5_PATH];
    iue_t experiment;

	hid_t experiment_id = H5Gopen(handle, IUS_INPUTFILE_PATH_EXPERIMENT, H5P_DEFAULT); // todo move "Experiment" to central place
    status |= iusHdf5ReadFloat(experiment_id, IUS_INPUTFILE_PATH_EXPERIMENT_SPEEDOFSOUND, &(speedOfSound));
    status |= iusHdf5ReadInt(experiment_id, IUS_INPUTFILE_PATH_EXPERIMENT_DATE, &(date));
    status |= iusHdf5ReadString(experiment_id, IUS_INPUTFILE_PATH_EXPERIMENT_DESCRIPTION, description);
	
	H5Gclose(experiment_id);
    if( status < 0 )
        return NULL;
    experiment = iusExperimentCreate(speedOfSound,date,description);
    return experiment;
}

