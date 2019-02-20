//
// Created by Ruijzendaal on 14/03/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ius.h>
#include <iusAcquisitionPrivate.h>
#include "iusAcquisitionADT.h"

iua_t iusAcquisitionCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    const char *pDescription  /**< Acquisition notes */
)
{
    IUS_ERR_EVAL_N_RETURN(speedOfSound < 0.0f, IUA_INVALID);
    IUS_ERR_EVAL_N_RETURN(date <= 0, IUA_INVALID);
    IusAcquisition *pAcquisition = (IusAcquisition *) calloc(1, sizeof(IusAcquisition));
    IUS_ERR_ALLOC_NULL_N_RETURN(pAcquisition, IusAcquisition, IUA_INVALID);
    pAcquisition->speedOfSound = speedOfSound;
    pAcquisition->date = date;
    if( pDescription == NULL )
        pAcquisition->description = "";
    else
        pAcquisition->description = strdup(pDescription);
    return pAcquisition;
}

int iusAcquisitionDelete
(
    iua_t acquisition
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, IUS_ERR_VALUE);
    free(acquisition->description);
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
	if (strcmp(reference->description, actual->description) != 0)
		return IUS_FALSE;
    return IUS_TRUE;
}

// getters
float iusAcquisitionGetSpeedOfSound
(
    iua_t acquisition
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, NAN);
    return acquisition->speedOfSound;
}

int iusAcquisitionGetDate
(
    iua_t acquisition
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, -1);
    return acquisition->date;
}

char * iusAcquisitionGetDescription
(
    iua_t acquisition
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, NULL);
    return acquisition->description;
}

// serialization
int iusAcquisitionSave
(
    iua_t acquisition,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

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

	if (acquisition_id == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_ACQUISITION);
        return IUS_ERR_VALUE;
    }


    status = iusHdf5WriteFloat(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND, &acquisition->speedOfSound, 1);
    status |= iusHdf5WriteInt(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DATE, &acquisition->date, 1);
    status |= iusHdf5WriteString(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DESCRIPTION, acquisition->description);

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

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUA_INVALID);

	hid_t acquisition_id = H5Gopen(handle, IUS_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT);
    status |= iusHdf5ReadFloat(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND, &(speedOfSound));
    status |= iusHdf5ReadInt(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DATE, &(date));
    status |= iusHdf5ReadString(acquisition_id, IUS_INPUTFILE_PATH_ACQUISITION_DESCRIPTION, description);
	
	H5Gclose(acquisition_id);
    if( status < 0 )
        return IUA_INVALID;
    acquisition = iusAcquisitionCreate(speedOfSound, date, description);
    return acquisition;
}

