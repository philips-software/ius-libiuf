//
// Created by nlv12901 on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusTransmitApodizationDictADT.h>
#include <iusTransmitApodizationPrivate.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableTransmitApodization, const char, struct HashableTransmitApodization)

// ADT
iutad_t iusTransmitApodizationDictCreate
(
)
{
	iutad_t dict = calloc(1, sizeof(IusTransmitApodizationDict));
	IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusTransmitApodizationDict, IUTAD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUS_FALSE;
	dict->keys = NULL;
	return dict;
}


int iusTransmitApodizationDictDeepDelete
(
	iutad_t dict
)
{
	if (dict == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUS_ERR_VALUE;
    }
	dict->deepDelete = IUS_TRUE;
	return iusTransmitApodizationDictDelete(dict);
}

static void iusTransmitApodizationDictDeleteKeys
(
    iutad_t dict
)
{
    if (dict->keys != NULL)
        free(dict->keys);
}

int iusTransmitApodizationDictDelete
(
	iutad_t dict
)
{
	struct hashmap_iter *iter;
	HashableTransmitApodization *iterElement;
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableTransmitApodization_hashmap_iter_get_data(iter);
		if(dict->deepDelete==IUS_TRUE)
		    iusTransmitApodizationDelete(iterElement->transmitApodization);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
    iusTransmitApodizationDictDeleteKeys(dict);
	free(dict);
	return IUS_E_OK;
}

static int iusTransmitApodizationDictSourceInTarget
(
	iutad_t source,
	iutad_t target
)
{
	HashableTransmitApodization *targetElement;
	HashableTransmitApodization *sourceElement;
	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		sourceElement = HashableTransmitApodization_hashmap_iter_get_data(iter);
		targetElement = HashableTransmitApodization_hashmap_get(&target->map, sourceElement->key);
		if (targetElement == NULL)
			return IUS_FALSE;

		if (iusTransmitApodizationCompare(sourceElement->transmitApodization, targetElement->transmitApodization) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusTransmitApodizationDictCompare
(
	iutad_t reference,
	iutad_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusTransmitApodizationDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusTransmitApodizationDictSourceInTarget(actual, reference);
}


size_t iusTransmitApodizationDictGetSize
(
	iutad_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return hashmap_size(&dict->map);
}

iuta_t iusTransmitApodizationDictGet
(
	iutad_t dict,
	char * key
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUTA_INVALID);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUTA_INVALID);
	HashableTransmitApodization *search;
	search = HashableTransmitApodization_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUTA_INVALID;
	}
	return search->transmitApodization;
}

char **iusTransmitApodizationDictGetKeys
(
    iutad_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->keys;
}

static int iusTransmitApodizationDictUpdateKeys
(
    iutad_t dict
)
{
    iusTransmitApodizationDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iusTransmitApodizationDictGetSize(dict);
    dict->keys = calloc(size+1, sizeof(char*));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableTransmitApodization *iterElement;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableTransmitApodization_hashmap_iter_get_data(iter);
        dict->keys[keyIndex] = iterElement->key;
    }
    dict->keys[keyIndex] = NULL;
    return IUS_E_OK;
}

int iusTransmitApodizationDictSet
(
	iutad_t dict,
	char * key,
	iuta_t member
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);
	HashableTransmitApodization *newMember = calloc(1, sizeof(HashableTransmitApodization));
	newMember->transmitApodization = member;
	strcpy(newMember->key, key);
	if (HashableTransmitApodization_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return iusTransmitApodizationDictUpdateKeys(dict);
}

herr_t iusTransmitApodizationDictSave
(
	iutad_t dict,
	hid_t handle
)
{
	herr_t status = 0;
	hid_t group_id;
	struct hashmap_iter *iter;

	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT);
	}

	if (group_id == H5I_INVALID_HID)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
		return IUS_ERR_VALUE;
	}
	status = 0;
	HashableTransmitApodization *transmitApodizationDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		transmitApodizationDictItem = HashableTransmitApodization_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, transmitApodizationDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusTransmitApodizationSave(transmitApodizationDictItem->transmitApodization, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}

#define MAX_NAME 1024

iutad_t iusTransmitApodizationDictLoad
(
	hid_t handle
)
{
	int status = 0;
	iuta_t transmitApodization;
	hsize_t i;
	char memberName[MAX_NAME];

	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUTAD_INVALID);
	hid_t groupId = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT);
	if (groupId == H5I_INVALID_HID)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
		return IUTAD_INVALID;
	}

	hsize_t nobj = 0;
	status = H5Gget_num_objs(groupId, &nobj);

	iutad_t dict = iusTransmitApodizationDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(groupId, i, memberName, (size_t)MAX_NAME);
		hid_t subgroupId = H5Gopen(groupId, memberName, H5P_DEFAULT);
		transmitApodization = iusTransmitApodizationLoad(subgroupId);
		status |= iusTransmitApodizationDictSet(dict, memberName, transmitApodization);
		H5Gclose(subgroupId);
	}
	H5Gclose(groupId);

	if (status != IUS_E_OK)
	{
		return IUTAD_INVALID;
	}
    dict->deepDelete = IUS_TRUE;
	return dict;
}

