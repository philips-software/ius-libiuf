
//
// Created by nlv09165 on 18/07/2018.
//
#include <stdlib.h>
#include <math.h>
#include <include/iusHLPatternListImp.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>
#include <include/iusHLPatternImp.h>
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

iupal_t iusHLPatternListCreate
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

int iusHLPatternListDelete
(
    iupal_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHLPatternListCompare
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
        if( iusHLPatternCompare( reference->pPatterns[index], actual->pPatterns[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHLPatternListGetSize
(
    iupal_t list
)
{
    return list->count;
}

iupa_t iusHLPatternListGet
(
    iupal_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->pPatterns[index];
}

int iusHLPatternListSet
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


iupal_t iusHLPatternListLoad
(
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int numPatterns,i;

	hid_t frameListId = H5Gopen(handle, "FrameList", H5P_DEFAULT);
    
	//sprintf(path, FRAMELISTSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(frameListId, "Size", &(numPatterns));
    if(status!=0) return IUPAL_INVALID;

    iupal_t patternList = iusHLPatternListCreate(numPatterns);
    iupa_t pattern;

    // Load patterns
    for (i=0;i < numPatterns;i++)
    {
        sprintf(path, "Pattern[%d]", i);
		hid_t patternId = H5Gopen(frameListId, path, H5P_DEFAULT);
        pattern = iusHLPatternLoad(patternId);
        if(pattern==IUPA_INVALID)
        {
            break;
        }
        iusHLPatternListSet(patternList,pattern,i);
    }

    return patternList;
}

IUS_BOOL iusHLPatternListFull
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

int iusHLPatternListSave
(
    iupal_t list,
	hid_t handle
)
{
    int status=0;
    int i,size;
    char path[IUS_MAX_HDF5_PATH];

    if(list == NULL)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;
    if(iusHLPatternListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;
	
	hid_t patternList_id;
	status = H5Gget_objinfo(handle, "Patterns", 0, NULL); // todo centralize the path "Sources"
	if (status != 0) // the group does not exist yet
	{
		patternList_id = H5Gcreate(handle, "Patterns", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		patternList_id = H5Gopen(handle, "Patterns", H5P_DEFAULT);
	}
    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iupa_t pattern;
    size = iusHLPatternListGetSize(list);
    //sprintf(path, FRAMELISTSIZEFMT, parentPath);
    status |= iusHdf5WriteInt(patternList_id, "Size", &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
		pattern = iusHLPatternListGet(list,i);
        if(pattern == IUPA_INVALID) continue;
		sprintf(path, "Pattern[%d]", i);
		hid_t pattern_id = H5Gcreate(patternList_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        //sprintf(path, FRAMELISTFMT, parentPath, i);
        status = iusHLPatternSave(pattern, pattern_id);
		H5Gclose(pattern_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(patternList_id);
    return status;
}

