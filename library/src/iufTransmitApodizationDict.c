//
// Created by nlv12901 on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufTransmitApodizationDictADT.h>
#include <iufTransmitApodizationPrivate.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableTransmitApodization, const char, struct HashableTransmitApodization)

// ADT
iutad_t iufTransmitApodizationDictCreate
(
)
{
	iutad_t dict = calloc(1, sizeof(IufTransmitApodizationDict));
	IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufTransmitApodizationDict, IUTAD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUF_FALSE;
	dict->kys = NULL;
	return dict;
}


int iufTransmitApodizationDictDeepDelete
(
	iutad_t dict
)
{
	if (dict == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "dict argument is NULL");
        return IUF_ERR_VALUE;
    }
	dict->deepDelete = IUF_TRUE;
	return iufTransmitApodizationDictDelete(dict);
}

static void iufTransmitApodizationDictDeleteKeys
(
    iutad_t dict
)
{
    if (dict->kys != NULL)
        free(dict->kys);
}

int iufTransmitApodizationDictDelete
(
	iutad_t dict
)
{
	struct hashmap_iter *iter;
	HashableTransmitApodization *iterElement;
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableTransmitApodization_hashmap_iter_get_data(iter);
		if(dict->deepDelete==IUF_TRUE)
		    iufTransmitApodizationDelete(iterElement->transmitApodization);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
    iufTransmitApodizationDictDeleteKeys(dict);
	free(dict);
	return IUF_E_OK;
}

static int iufTransmitApodizationDictSourceInTarget
(
	iutad_t source,
	iutad_t target
)
{
	HashableTransmitApodization *targetElement;
	HashableTransmitApodization *sourceElement;
	IUF_BOOL sourceInTarget = IUF_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		sourceElement = HashableTransmitApodization_hashmap_iter_get_data(iter);
		targetElement = HashableTransmitApodization_hashmap_get(&target->map, sourceElement->key);
		if (targetElement == NULL)
			return IUF_FALSE;

		if (iufTransmitApodizationCompare(sourceElement->transmitApodization, targetElement->transmitApodization) == IUF_FALSE)
			return IUF_FALSE;
		sourceInTarget = IUF_TRUE;
	}

	return sourceInTarget;
}

// operations
int iufTransmitApodizationDictCompare
(
	iutad_t reference,
	iutad_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUF_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUF_TRUE;

	// check if elements of source hash are in target hash
	if (iufTransmitApodizationDictSourceInTarget(reference, actual) == IUF_FALSE)
		return IUF_FALSE;

	// check if elements of target hash are in source hash
	return iufTransmitApodizationDictSourceInTarget(actual, reference);
}


size_t iufTransmitApodizationDictGetSize
(
	iutad_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return hashmap_size(&dict->map);
}

iuta_t iufTransmitApodizationDictGet
(
	iutad_t dict,
	char * key
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUTA_INVALID);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUTA_INVALID);
	HashableTransmitApodization *search;
	search = HashableTransmitApodization_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUTA_INVALID;
	}
	return search->transmitApodization;
}

char **iufTransmitApodizationDictGetKeys
(
    iutad_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufTransmitApodizationDictUpdateKeys
(
    iutad_t dict
)
{
    iufTransmitApodizationDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufTransmitApodizationDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableTransmitApodization *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableTransmitApodization_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufTransmitApodizationDictSet
(
	iutad_t dict,
	char * key,
	iuta_t member
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
	HashableTransmitApodization *newMember = calloc(1, sizeof(HashableTransmitApodization));
	newMember->transmitApodization = member;
	strcpy(newMember->key, key);
	if (HashableTransmitApodization_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUF_ERR_VALUE;
	}
	return iufTransmitApodizationDictUpdateKeys(dict);
}

herr_t iufTransmitApodizationDictSave
(
	iutad_t dict,
	hid_t handle
)
{
	herr_t status = 0;
	hid_t group_id;
	struct hashmap_iter *iter;

	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
	status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT);
	}

	if (group_id == H5I_INVALID_HID)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
		return IUF_ERR_VALUE;
	}
	status = 0;
	HashableTransmitApodization *transmitApodizationDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		transmitApodizationDictItem = HashableTransmitApodization_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, transmitApodizationDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iufTransmitApodizationSave(transmitApodizationDictItem->transmitApodization, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}

#define MAX_NAME 1024

iutad_t iufTransmitApodizationDictLoad
(
	hid_t handle
)
{
	int status = 0;
	iuta_t transmitApodization;
	hsize_t i;
	char memberName[MAX_NAME];

	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUTAD_INVALID);
	hid_t groupId = H5Gopen(handle, IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT);
	if (groupId == H5I_INVALID_HID)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
		return IUTAD_INVALID;
	}

	hsize_t nobj = 0;
	status = H5Gget_num_objs(groupId, &nobj);

	iutad_t dict = iufTransmitApodizationDictCreate();
	for (i = 0; i < nobj && status == IUF_E_OK; i++)
	{
		H5Gget_objname_by_idx(groupId, i, memberName, (size_t)MAX_NAME);
		hid_t subgroupId = H5Gopen(groupId, memberName, H5P_DEFAULT);
		transmitApodization = iufTransmitApodizationLoad(subgroupId);
		status |= iufTransmitApodizationDictSet(dict, memberName, transmitApodization);
		H5Gclose(subgroupId);
	}
	H5Gclose(groupId);

	if (status != IUF_E_OK)
	{
		return IUTAD_INVALID;
	}
    dict->deepDelete = IUF_TRUE;
	return dict;
}

