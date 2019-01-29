//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusDemodulationPrivate.h>

// ADT
struct HashableDemodulation
{
	iudm_t demodulation;
	char key[256];
};

typedef struct HashableDemodulation HashableDemodulation;

struct IusDemodulationDict
{
	struct hashmap map;
	IUS_BOOL deepDelete;
    char **keys;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableDemodulation, const char, struct HashableDemodulation)

// ADT
iudmd_t iusDemodulationDictCreate
(
)
{
	iudmd_t dict = calloc(1, sizeof(IusDemodulationDict));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusDemodulationDict, IUDMD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUS_FALSE;
    dict->keys = NULL;
	return dict;
}

int iusDemodulationDictDeepDelete
(
	iudmd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	dict->deepDelete = IUS_TRUE;
	return iusDemodulationDictDelete(dict);
}

static void iusDemodulationDictDeleteKeys
(
    iudmd_t dict
)
{
    if (dict->keys != NULL)
        free(dict->keys);
}

int iusDemodulationDictDelete
(
	iudmd_t dict
)
{
	HashableDemodulation *iterElement;
	struct hashmap_iter *iter;

    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
		if (dict->deepDelete == IUS_TRUE)
			iusDemodulationDelete(iterElement->demodulation);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
    iusDemodulationDictDeleteKeys(dict);
	free(dict);
	return IUS_E_OK;
}


static int iusDemodulationDictSourceInTarget
(
	iudmd_t source,
	iudmd_t target
)
{
	iudm_t targetElement;
	iudm_t sourceElement;
	HashableDemodulation *iterElement;

	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
		sourceElement = iterElement->demodulation;
		targetElement = iusDemodulationDictGet(target, iterElement->key);
		if (targetElement == IUDM_INVALID)
			return IUS_FALSE;

		if (iusDemodulationCompare(sourceElement, targetElement) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusDemodulationDictCompare
(
	iudmd_t reference,
	iudmd_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusDemodulationDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusDemodulationDictSourceInTarget(actual, reference);
}


size_t iusDemodulationDictGetSize
(
	iudmd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return (int)hashmap_size(&dict->map);
}

iudm_t iusDemodulationDictGet
(
	iudmd_t dict,
	char * key
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUDM_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(key, IUDM_INVALID);
    HashableDemodulation * search;
	search = HashableDemodulation_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IUDM_INVALID;
    }
	return search->demodulation;
}

char **iusDemodulationDictGetKeys
(
    iudmd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->keys;
}

static int iusDemodulationDictUpdateKeys
(
    iudmd_t dict
)
{
    iusDemodulationDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iusDemodulationDictGetSize(dict);
    dict->keys = calloc(size+1, sizeof(char*));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableDemodulation *iterElement;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
        dict->keys[keyIndex] = iterElement->key;
    }
    dict->keys[keyIndex] = NULL;
    return IUS_E_OK;
}

int iusDemodulationDictSet
(
	iudmd_t dict,
	char * key,
	iudm_t member
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);
    HashableDemodulation *newMember = calloc(1, sizeof(HashableDemodulation));
	newMember->demodulation = member;
	strcpy(newMember->key, key);
	if (HashableDemodulation_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return iusDemodulationDictUpdateKeys(dict);
}


// serialization
int iusDemodulationDictSave
(
	iudmd_t dict,
	hid_t handle
)
{
	int status = 0;
	struct hashmap_iter *iter;
	hid_t group_id;

    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	status = H5Gget_objinfo(handle, IUS_IQFILE_PATH_DEMODULATIONDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_IQFILE_PATH_DEMODULATIONDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_IQFILE_PATH_DEMODULATIONDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_IQFILE_PATH_DEMODULATIONDICT);
        return IUS_ERR_VALUE;
    }

    status = 0;
	HashableDemodulation *demodulationDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		demodulationDictItem = HashableDemodulation_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, demodulationDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusDemodulationSave(demodulationDictItem->demodulation, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}


iudmd_t iusDemodulationDictLoad
(
	hid_t handle
)
{
	int i;
	int status = 0;
	char member_name[IUS_MAX_HDF5_PATH];

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDMD_INVALID);
	hid_t grpid = H5Gopen(handle, IUS_IQFILE_PATH_DEMODULATIONDICT, H5P_DEFAULT);
	if (handle == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iudmd_t dict = iusDemodulationDictCreate();
	for (i = 0; i < (int)nobj; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, member_name, (size_t)IUS_MAX_HDF5_PATH);

		hid_t settings_id = H5Gopen(grpid, member_name, H5P_DEFAULT);
		iudm_t demodulation = iusDemodulationLoad(settings_id);
		status = iusDemodulationDictSet(dict, member_name, demodulation);
		H5Gclose(settings_id);
	}
	H5Gclose(grpid);

	if (status != IUS_E_OK)
	{
		return NULL;
	}
	dict->deepDelete = IUS_TRUE;
	return dict;
}
