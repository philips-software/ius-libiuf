//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashmap.h>

#include <ius.h>
#include <iusPulsePrivate.h>
#include <iusParametricPulsePrivate.h>
#include <iusNonParametricPulsePrivate.h>

// ADT
struct HashablePulse
{
    iup_t pulse;
    char key[256];
} ;

typedef struct HashablePulse HashablePulse;

struct IusPulseDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
} ;

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashablePulse, const char, struct HashablePulse)

// ADT
iupd_t iusPulseDictCreate
(
)
{
    iupd_t dict = calloc(1, sizeof(IusPulseDict));
    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for IusPulseDict");
        return NULL;
    }

    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUS_FALSE;
    return dict;
}

int iusPulseDictDeepDelete
(
    iupd_t dict
)
{
    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUS_ERR_VALUE;
    }
    dict->deepDelete = IUS_TRUE;
    return iusPulseDictDelete(dict);
}

int iusPulseDictDelete
(
  iupd_t dict
)
{
    struct hashmap_iter *iter;
    HashablePulse *iterElement;
    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUS_ERR_VALUE;
    }

    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashablePulse_hashmap_iter_get_data(iter);
        if (dict->deepDelete == IUS_TRUE)
            iusPulseDelete(iterElement->pulse);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    free(dict);
    return IUS_E_OK;
}


static int iusPulseDictSourceInTarget
(
    iupd_t source,
    iupd_t target
)
{
    iup_t targetElement;
    iup_t sourceElement;
    HashablePulse *iterElement;

    IUS_BOOL sourceInTarget = IUS_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashablePulse_hashmap_iter_get_data(iter);
      sourceElement=iterElement->pulse;
      targetElement = iusPulseDictGet(target, iterElement->key);
      if( targetElement == IUP_INVALID)
        return IUS_FALSE;

      if( iusPulseCompare(sourceElement, targetElement) == IUS_FALSE )
        return IUS_FALSE;
      sourceInTarget = IUS_TRUE;
    }

  return sourceInTarget;
}

// operations
int iusPulseDictCompare
(
  iupd_t reference,
  iupd_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUS_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUS_TRUE;

    // check if elements of source hash are in target hash
    if( iusPulseDictSourceInTarget(reference,actual) == IUS_FALSE )
        return IUS_FALSE;

    // check if elements of target hash are in source hash
    return iusPulseDictSourceInTarget(actual,reference);
}


int iusPulseDictGetSize
(
    iupd_t dict
)
{
    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return -1;
    }
    return (int) hashmap_size(&dict->map);
}

iup_t iusPulseDictGet
(
    iupd_t dict,
    char * key
)
{
    HashablePulse * search;
    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUP_INVALID;
    }

    if (key == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "key argument is NULL");
        return IUP_INVALID;
    }
    search = HashablePulse_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IUP_INVALID;
    }
    return search->pulse;
}

int iusPulseDictSet
(
    iupd_t dict,
    char * key,
    iup_t member
)
{
    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUS_ERR_VALUE;
    }

    if (key == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "key argument is NULL");
        return IUS_ERR_VALUE;
    }

    HashablePulse *newMember = calloc(1, sizeof(HashablePulse));
    newMember->pulse = member;
    strcpy(newMember->key,key);
    if (HashablePulse_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
        free(newMember);
        return IUS_ERR_VALUE;
    }
    return IUS_E_OK;
}


// serialization
int iusPulseDictSave
(
    iupd_t dict,
    hid_t handle
)
{
    int status=0;
    struct hashmap_iter *iter;

    if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUS_ERR_VALUE;
    }

    if (handle == H5I_INVALID_HID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "handle argument is invalid");
        return IUS_ERR_VALUE;
    }

    hid_t group_id;
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_PULSEDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_PULSEDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_PULSEDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
        return IUS_ERR_VALUE;
    }

	status = 0;
    HashablePulse *pulseDictItem;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter && status == IUS_E_OK; iter = hashmap_iter_next(&dict->map, iter))
    {
		pulseDictItem = HashablePulse_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, pulseDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (subgroup_id <= 0)
        {
            H5Gclose(group_id);
            return IUS_ERR_VALUE;
        }
        status |= iusPulseSave(pulseDictItem->pulse, subgroup_id);
		status |=  H5Gclose(subgroup_id);
    }
    status |= H5Gclose(group_id );
    return status;
}


#define MAX_NAME 1024

iupd_t iusPulseDictLoad
(
	hid_t handle
)
{
	int i;
	int status;
	//char path[IUS_MAX_HDF5_PATH];
	char memb_name[MAX_NAME];

    if (handle == H5I_INVALID_HID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "handle argument is invalid");
        return IUPD_INVALID;
    }

    hid_t grpid = H5Gopen(handle, IUS_INPUTFILE_PATH_PULSEDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_PULSEDICT);
        return IUPD_INVALID;
    }

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iupd_t dict = iusPulseDictCreate();
    for (i = 0; i < (int) nobj && status == IUS_E_OK; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i, memb_name, (size_t) MAX_NAME);
		hid_t pulse_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
        iup_t pulse = iusPulseLoad(pulse_id); //note iusPulseLoad expect handle, not grpid!
		status |= H5Gclose(pulse_id);
        status |= iusPulseDictSet(dict, memb_name, pulse);
    }
    H5Gclose(grpid);
    if( status != IUS_E_OK )
    {
        return IUPD_INVALID;
    }
    dict->deepDelete = IUS_TRUE;
    return dict;
}
