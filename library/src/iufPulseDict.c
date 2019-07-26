#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufPulsePrivate.h>
#include <iufParametricPulsePrivate.h>
#include <iufPulseDictADT.h>
#include <iufNonParametricPulsePrivate.h>


/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashablePulse, const char, struct HashablePulse)

// ADT
iupd_t iufPulseDictCreate
(
)
{
    iupd_t dict = calloc(1, sizeof(IufPulseDict));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufPulseDict, IUPD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
    return dict;
}

static void iufPulseDictDeleteKeys
(
    iupd_t dict
)
{
    if (dict->kys != NULL)
    free(dict->kys);
}

int iufPulseDictDeepDelete
(
    iupd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    dict->deepDelete = IUF_TRUE;
    return iufPulseDictDelete(dict);
}

int iufPulseDictDelete
(
  iupd_t dict
)
{
    struct hashmap_iter *iter;
    HashablePulse *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);

    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashablePulse_hashmap_iter_get_data(iter);
        if (dict->deepDelete == IUF_TRUE)
            iufPulseDelete(iterElement->pulse);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    iufPulseDictDeleteKeys(dict);
    free(dict);
    return IUF_E_OK;
}


static int iufPulseDictSourceInTarget
(
    iupd_t source,
    iupd_t target
)
{
    iup_t targetElement;
    iup_t sourceElement;
    HashablePulse *iterElement;

    IUF_BOOL sourceInTarget = IUF_FALSE;
    struct hashmap_iter *iter;

    // iterate over source list elements using the hash double linked list
    for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
      iterElement = HashablePulse_hashmap_iter_get_data(iter);
      sourceElement=iterElement->pulse;
      targetElement = iufPulseDictGet(target, iterElement->key);
      if( targetElement == IUP_INVALID)
        return IUF_FALSE;

      if( iufPulseCompare(sourceElement, targetElement) == IUF_FALSE )
        return IUF_FALSE;
      sourceInTarget = IUF_TRUE;
    }

  return sourceInTarget;
}

// operations
int iufPulseDictCompare
(
  iupd_t reference,
  iupd_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( hashmap_size(&reference->map) != hashmap_size(&actual->map) ) return IUF_FALSE;
    if( hashmap_size(&reference->map) == 0) return IUF_TRUE;

    // check if elements of source hash are in target hash
    if( iufPulseDictSourceInTarget(reference,actual) == IUF_FALSE )
        return IUF_FALSE;

    // check if elements of target hash are in source hash
    return iufPulseDictSourceInTarget(actual,reference);
}


size_t iufPulseDictGetSize
(
    iupd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
    return (int) hashmap_size(&dict->map);
}

iup_t iufPulseDictGet
(
    iupd_t dict,
    char * key
)
{
    HashablePulse * search;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUP_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUP_INVALID);
    search = HashablePulse_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IUP_INVALID;
    }
    return search->pulse;
}

char **iufPulseDictGetKeys
(
    iupd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufPulseDictUpdateKeys
(
    iupd_t  dict
)
{
    iufPulseDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufPulseDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashablePulse *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashablePulse_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufPulseDictSet
(
    iupd_t dict,
    char * key,
    iup_t member
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);

    HashablePulse *newMember = calloc(1, sizeof(HashablePulse));
    newMember->pulse = member;
    strcpy(newMember->key,key);
    if (HashablePulse_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
        free(newMember);
        return IUF_ERR_VALUE;
    }
    return iufPulseDictUpdateKeys(dict);
}


// serialization
int iufPulseDictSave
(
    iupd_t dict,
    hid_t handle
)
{
    int status=0;
    struct hashmap_iter *iter;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    hid_t group_id;
	status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_PULSEDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_PULSEDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_PULSEDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
        return IUF_ERR_VALUE;
    }

	status = 0;
    HashablePulse *pulseDictItem;

    // iterate over source list elements and save'em
    for (iter = hashmap_iter(&dict->map); iter && status == IUF_E_OK; iter = hashmap_iter_next(&dict->map, iter))
    {
		pulseDictItem = HashablePulse_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, pulseDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (subgroup_id <= 0)
        {
            H5Gclose(group_id);
            return IUF_ERR_VALUE;
        }
        status |= iufPulseSave(pulseDictItem->pulse, subgroup_id);
		status |=  H5Gclose(subgroup_id);
    }
    status |= H5Gclose(group_id );
    return status;
}


#define MAX_NAME 1024

iupd_t iufPulseDictLoad
(
	hid_t handle
)
{
	int i;
	int status;
	//char path[IUF_MAX_HDF5_PATH];
	char memb_name[MAX_NAME];

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPD_INVALID);
    hid_t grpid = H5Gopen(handle, IUF_INPUTFILE_PATH_PULSEDICT, H5P_DEFAULT);
    if (grpid == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_PULSEDICT);
        return IUPD_INVALID;
    }

    hsize_t nobj;
    status = H5Gget_num_objs(grpid, &nobj);

    iupd_t dict = iufPulseDictCreate();
    for (i = 0; i < (int) nobj && status == IUF_E_OK; i++)
    {
        H5Gget_objname_by_idx(grpid, (hsize_t) i, memb_name, (size_t) MAX_NAME);
		hid_t pulse_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
        iup_t pulse = iufPulseLoad(pulse_id); //note iufPulseLoad expect handle, not grpid!
		status |= H5Gclose(pulse_id);
        status |= iufPulseDictSet(dict, memb_name, pulse);
    }
    H5Gclose(grpid);
    if( status != IUF_E_OK )
    {
        return IUPD_INVALID;
    }
    dict->deepDelete = IUF_TRUE;
    return dict;
}
