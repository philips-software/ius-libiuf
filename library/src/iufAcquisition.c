#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iuf.h>
#include <iufAcquisitionPrivate.h>
#include "iufAcquisitionADT.h"

iua_t iufAcquisitionCreate
(
    float speedOfSound, /**< speed of sound in m/s */
    int date,           /**< interger concatenation of year-month-day  */
    const char *pDescription  /**< Acquisition notes */
)
{
    IUF_ERR_EVAL_N_RETURN(speedOfSound < 0.0f, IUA_INVALID);
    IUF_ERR_EVAL_N_RETURN(date <= 0, IUA_INVALID);
    IufAcquisition *pAcquisition = (IufAcquisition *) calloc(1, sizeof(IufAcquisition));
    IUF_ERR_ALLOC_NULL_N_RETURN(pAcquisition, IufAcquisition, IUA_INVALID);
    pAcquisition->speedOfSound = speedOfSound;
    pAcquisition->date = date;
    if( pDescription == NULL )
        pAcquisition->description = "";
    else
        pAcquisition->description = strdup(pDescription);
    return pAcquisition;
}

int iufAcquisitionDelete
(
    iua_t acquisition
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, IUF_ERR_VALUE);
    free(acquisition->description);
    free(acquisition);
    return IUF_E_OK;
}

IUF_BOOL iufAcquisitionCompare
(
    iua_t reference,
    iua_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL )
        return IUF_FALSE;

    if( reference->date != actual->date )
        return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->speedOfSound, actual->speedOfSound) == IUF_FALSE )
        return IUF_FALSE;
    if (strcmp(reference->description, actual->description) != 0)
        return IUF_FALSE;
    return IUF_TRUE;
}

// getters
float iufAcquisitionGetSpeedOfSound
(
    iua_t acquisition
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, NAN);
    return acquisition->speedOfSound;
}

int iufAcquisitionGetDate
(
    iua_t acquisition
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, -1);
    return acquisition->date;
}

char * iufAcquisitionGetDescription
(
    iua_t acquisition
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, NULL);
    return acquisition->description;
}

// serialization
int iufAcquisitionSave
(
    iua_t acquisition,
    hid_t handle
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    hid_t acquisition_id;
    status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_ACQUISITION, 0, NULL);
    if (status != 0) // the group does not exist yet
    {
        acquisition_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        acquisition_id = H5Gopen(handle, IUF_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT);
    }

    if (acquisition_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_ACQUISITION);
        return IUF_ERR_VALUE;
    }


    status = iufHdf5WriteFloat(acquisition_id, IUF_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND, &acquisition->speedOfSound, 1);
    status |= iufHdf5WriteInt(acquisition_id, IUF_INPUTFILE_PATH_ACQUISITION_DATE, &acquisition->date, 1);
    status |= iufHdf5WriteString(acquisition_id, IUF_INPUTFILE_PATH_ACQUISITION_DESCRIPTION, acquisition->description);

    H5Gclose(acquisition_id);
    return status;
}

iua_t iufAcquisitionLoad
(
    hid_t handle
)
{
    int status = 0;
    float speedOfSound;
    int date;
    char description[IUF_MAX_HDF5_PATH];
    iua_t acquisition;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUA_INVALID);

    hid_t acquisition_id = H5Gopen(handle, IUF_INPUTFILE_PATH_ACQUISITION, H5P_DEFAULT);
    status |= iufHdf5ReadFloat(acquisition_id, IUF_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND, &(speedOfSound));
    status |= iufHdf5ReadInt(acquisition_id, IUF_INPUTFILE_PATH_ACQUISITION_DATE, &(date));
    status |= iufHdf5ReadString(acquisition_id, IUF_INPUTFILE_PATH_ACQUISITION_DESCRIPTION, description);
    
    H5Gclose(acquisition_id);
    if( status < 0 )
        return IUA_INVALID;
    acquisition = iufAcquisitionCreate(speedOfSound, date, description);
    return acquisition;
}

