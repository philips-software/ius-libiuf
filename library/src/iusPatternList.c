
//
// Created by nlv09165 on 18/07/2018.
//
#include <stdlib.h>
#include <math.h>
#include <include/iusPatternListImp.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>
#include <include/iusPatternImp.h>
#include <include/iusHDF5.h>


#define FRAMELISTFMT "%s/Pattern[%d]"
#define FRAMELISTSIZEFMT "%s/Size"


// ADT
struct IusPatternList
{
    int count;
    iupa_t *   pPatterns ;
} ;

// ADT

iupal_t iusPatternListCreate
(
    int numPatterns
)
{
    iupal_t list = calloc(1, sizeof(IusPatternList));
    if(list!=NULL)
    {
        list->count = numPatterns;
        list->pPatterns = (iupa_t *) calloc((size_t)numPatterns, sizeof(iupa_t));
        if( list->pPatterns == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusPatternListDelete
(
    iupal_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusPatternListCompare
(
    iupal_t reference,
    iupal_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusPatternCompare( reference->pPatterns[index], actual->pPatterns[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusPatternListGetSize
(
    iupal_t list
)
{
    return list->count;
}

iupa_t iusPatternListGet
(
    iupal_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->pPatterns[index];
}

int iusPatternListSet
(
    iupal_t list,
    iupa_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
    list->pPatterns[index] = member;
    return IUS_E_OK;
}


iupal_t iusPatternListLoad
(
    hid_t handle,
    const char *parentPath
)
{
    char path[IUS_MAX_HDF5_PATH];
    int numPatterns,i;
    sprintf(path, FRAMELISTSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(handle, path, &(numPatterns));
    if(status!=0) return IUPAL_INVALID;

    iupal_t patternList = iusPatternListCreate(numPatterns);
    iupa_t sourceElement;

    // Load patterns
    for (i=0;i < numPatterns;i++)
    {
        sprintf(path, FRAMELISTFMT, parentPath, i);
        sourceElement = iusPatternLoad(handle,path);
        if(sourceElement==IUPA_INVALID)
        {
            break;
        }
        iusPatternListSet(patternList,sourceElement,i);
    }

    return patternList;
}

IUS_BOOL iusPatternListFull
(
    iupal_t list
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < list->count;i++)
    {
        if(list->pPatterns[i] == IUPA_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}

int iusPatternListSave
(
    iupal_t list,
    const char *parentPath,
    hid_t handle
)
{
    int status=0;
    int i,size;
    char path[IUS_MAX_HDF5_PATH];

    if(list == NULL)
        return IUS_ERR_VALUE;
    if(parentPath == NULL || handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;
    if(iusPatternListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iupa_t sourceElement;
    size = iusPatternListGetSize(list);
    sprintf(path, FRAMELISTSIZEFMT, parentPath);
    status |= iusHdf5WriteInt(handle, path, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = iusPatternListGet(list,i);
        if(sourceElement == IUPA_INVALID) continue;

        sprintf(path, FRAMELISTFMT, parentPath, i);
        status = iusPatternSave(sourceElement,path,group_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id );
    return status;
}

