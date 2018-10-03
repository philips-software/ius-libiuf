//
// Created by nlv09165 on 30/07/2018.
//
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <include/iusInputFileStructure.h>
#include <include/iusSourceImp.h>
#include <include/iusPositionImp.h>
#include <include/ius2DParametricSource.h>
#include <include/iusHDF5.h>

struct Ius2DParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius2DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
} ;


// ADT
iu2dps_t ius2DParametricSourceCreate
(
    char *pLabel,
    int numLocations,
    float fNumber,
    float angularDelta,
    float startAngle
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu2dps_t created = calloc(1,sizeof(Ius2DParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc(numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->locationCount = numLocations;
    created->angularDelta = angularDelta;
    created->startAngle = startAngle;
    created->fNumber = fNumber;
    return created;
}

int ius2DParametricSourceDelete
(
    iu2dps_t ius2DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DParametricSource != NULL)
    {
        free(ius2DParametricSource);
        ius2DParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int ius2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startAngle, actual->startAngle) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->angularDelta, actual->angularDelta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->locationCount; i++ )
    {
        if( ius2DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius2DParametricSourceGetFNumber
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->fNumber;
}

float ius2DParametricSourceGetAngularDelta
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->angularDelta;
}

float ius2DParametricSourceGetStartAngle
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->startAngle;
}

iu2dp_t ius2DParametricSourceGetPosition
(
    iu2dps_t ius2DParametricSource,
    int index
)
{
    if ( ius2DParametricSource == NULL  ) return IU2DP_INVALID;
    if ( index >= ius2DParametricSource->locationCount || index < 0) return IU2DP_INVALID;
    return &ius2DParametricSource->pLocations[index];
}

// Setters
int ius2DParametricSourceSetPosition
(
    iu2dps_t ius2DParametricSource,
    iu2dp_t  pos,
    int index
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    if (pos == NULL) return IUS_ERR_VALUE;
    if (index >= ius2DParametricSource->locationCount) return IUS_ERR_VALUE;

    ius2DParametricSource->pLocations[index] = *pos;
    return IUS_E_OK;
}

int ius2DParametricSourceSetFNumber
(
    iu2dps_t ius2DParametricSource,
    float FNumber
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->fNumber = FNumber;
    return IUS_E_OK;
}

int ius2DParametricSourceSetAngularDelta
(
    iu2dps_t ius2DParametricSource,
    float angularDelta
    )
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->angularDelta = angularDelta;
    return IUS_E_OK;
}

int ius2DParametricSourceSetStartAngle
(
    iu2dps_t ius2DParametricSource,
    float startAngle
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->startAngle = startAngle;
    return IUS_E_OK;
}


// serialization
static int ius2DParametricSourceSaveLocations
(
    iu2dps_t pSource,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t sourceElement;

    int i,size = pSource->locationCount;
    int status = iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(size), 1);

//  iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &pSource->pLocations[i];
        if(sourceElement == IU2DP_INVALID) continue;
		sprintf(path, IUS_INPUTFILE_PATH_SOURCE_LOCATION, i);
		location_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = ius2DPositionSave(sourceElement, location_id);
		H5Gclose(location_id);
        if(status != IUS_E_OK) break;
    }
    return status;
}

static int ius2DParametricSourceLoadLocations
(
    iu2dps_t source,
    hid_t handle
)
{
    int p,status=IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t pos;

    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_SOURCE_LOCATION, p);
		hid_t location_id = H5Gopen(handle, path, H5P_DEFAULT);
        pos = ius2DPositionLoad(location_id);
        if (pos == IU2DP_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
		H5Gclose(location_id);
        ius2DParametricSourceSetPosition(source, pos, p);
    }
    return status;
};

int ius2DParametricSourceSave
(
    iu2dps_t source,
    hid_t handle
)
{
    int status=0;
    const int verbose = 1;

    // Base
    status = iusBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(source->fNumber), 1, verbose);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_ANGULARDELTA, &(source->angularDelta), 1, verbose);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTANGLE, &(source->startAngle), 1, verbose);

    // Save locations
	// todo: should we make a Locations group here?
    status |= ius2DParametricSourceSaveLocations(source, handle);
    return status;
}


iu2dps_t ius2DParametricSourceLoad
(
    hid_t handle,
    char *label
)
{
    int status = 0;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
    int locationCount;
    iu2dps_t  source;

    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_ANGULARDELTA, &(angularDelta));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_STARTANGLE, &(startAngle));
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(locationCount));
    if (status < 0)
        return NULL;

    source = ius2DParametricSourceCreate(label,locationCount,fNumber,angularDelta,startAngle);

    status = ius2DParametricSourceLoadLocations(source, handle);
    if (status <-0)
        return NULL;
    return source;
}
