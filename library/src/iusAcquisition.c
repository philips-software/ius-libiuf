//
// Created by Ruijzendaal on 14/03/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ius.h>
#include <iusAcquisitionPrivate.h>

// ADT
/** \brief An Ultrasound acquisition is identified by a date and a description, also the speed of sound has been determined */
struct IusAcquisition
{
    float  speedOfSound;    /**< speed of sound in m/s */
    int    date;            /**< interger concatenation of year-month-day e.g. 20160123 for 23th Jan 2016 */
    char * pDescription;    /**< Acquisition notes */
} ;

iua_t iusAcquisitionCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    const char *pDescription  /**< Acquisition notes */
)
{
    if ( speedOfSound < 0.0f ) return IUA_INVALID;
    if ( date <= 0 ) return IUA_INVALID;

    IusAcquisition *pAcquisition = (IusAcquisition *) calloc(1, sizeof(IusAcquisition));
    pAcquisition->speedOfSound = speedOfSound;
    pAcquisition->date = date;
    if( pDescription == NULL )
        pAcquisition->pDescription = "";
    else
        pAcquisition->pDescription = strdup(pDescription);
    return pAcquisition;
}

int iusAcquisitionDelete
(
    iua_t acquisition
)
{
    if( acquisition == NULL ) return IUS_ERR_VALUE;
    free(acquisition->pDescription);
    free(acquisition);
    return IUS_E_OK;
}

IUS_BOOL iusAcquisitionCompare
(
    iua_t reference,
    iua_t actual
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
float iusAcquisitionGetSpeedOfSound
(
    iua_t acquisition
)
{
    if( acquisition == NULL ) return NAN;
    return acquisition->speedOfSound;
}

int iusAcquisitionGetDate
(
    iua_t acquisition
)
{
    if( acquisition == NULL ) return -1;
    return acquisition->date;
}

char * iusAcquisitionGetDescription
(
    iua_t acquisition
)
{
    if( acquisition == NULL ) return NULL;
    return acquisition->pDescription;
}

#if 0
// old routines
int LF_copyAcquisitionData
(
    iua_t pDst,
    iua_t pSrc
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
int iusAcquisitionSave
(
    iua_t acquisition,
    hid_t handle
)
{
    int status=0;

	hid_t acquisition_id;
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_ACQUISITION, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		acquisition_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		acquisition_id = H5Gopen(handle, IUS_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT);
	}
	if (acquisition_id > 0) status = 0;
    status |= iusHdf5WriteFloat(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND, &acquisition->speedOfSound, 1);
    status |= iusHdf5WriteInt(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DATE, &acquisition->date, 1);
    status |= iusHdf5WriteString(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DESCRIPTION, acquisition->pDescription);

	H5Gclose(acquisition_id);
    return status;
}

iua_t iusAcquisitionLoad
(
    hid_t handle
)
{
    int status = 0;
    float speedOfSound;
    int date;
    char description[IUS_MAX_HDF5_PATH];
    iua_t acquisition;

	hid_t acquisition_id = H5Gopen(handle, IUS_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT);
    status |= iusHdf5ReadFloat(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND, &(speedOfSound));
    status |= iusHdf5ReadInt(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DATE, &(date));
    status |= iusHdf5ReadString(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DESCRIPTION, description);
	
	H5Gclose(acquisition_id);
    if( status < 0 )
        return NULL;
    acquisition = iusAcquisitionCreate(speedOfSound, date, description);
    return acquisition;
}

