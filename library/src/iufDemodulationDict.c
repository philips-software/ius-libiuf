//
// Created by nlv09165 on 02/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufIqFileStructure.h>
#include <iufDemodulationPrivate.h>
#include <iufDemodulationDictADT.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableDemodulation, const char, struct HashableDemodulation)

// ADT
iudmd_t iufDemodulationDictCreate
(
)
{
	iudmd_t dict = calloc(1, sizeof(IufDemodulationDict));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufDemodulationDict, IUDMD_INVALID);
    hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
    dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
	return dict;
}

int iufDemodulationDictDeepDelete
(
	iudmd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	dict->deepDelete = IUF_TRUE;
	return iufDemodulationDictDelete(dict);
}

static void iufDemodulationDictDeleteKeys
(
    iudmd_t dict
)
{
    if (dict->kys != NULL)
        free(dict->kys);
}

int iufDemodulationDictDelete
(
	iudmd_t dict
)
{
	HashableDemodulation *iterElement;
	struct hashmap_iter *iter;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
		if (dict->deepDelete == IUF_TRUE)
			iufDemodulationDelete(iterElement->demodulation);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
    iufDemodulationDictDeleteKeys(dict);
	free(dict);
	return IUF_E_OK;
}


static int iufDemodulationDictSourceInTarget
(
	iudmd_t source,
	iudmd_t target
)
{
	iudm_t targetElement;
	iudm_t sourceElement;
	HashableDemodulation *iterElement;

	IUF_BOOL sourceInTarget = IUF_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
		sourceElement = iterElement->demodulation;
		targetElement = iufDemodulationDictGet(target, iterElement->key);
		if (targetElement == IUDM_INVALID)
			return IUF_FALSE;

		if (iufDemodulationCompare(sourceElement, targetElement) == IUF_FALSE)
			return IUF_FALSE;
		sourceInTarget = IUF_TRUE;
	}

	return sourceInTarget;
}

// operations
int iufDemodulationDictCompare
(
	iudmd_t reference,
	iudmd_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUF_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUF_TRUE;

	// check if elements of source hash are in target hash
	if (iufDemodulationDictSourceInTarget(reference, actual) == IUF_FALSE)
		return IUF_FALSE;

	// check if elements of target hash are in source hash
	return iufDemodulationDictSourceInTarget(actual, reference);
}


size_t iufDemodulationDictGetSize
(
	iudmd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return (int)hashmap_size(&dict->map);
}

iudm_t iufDemodulationDictGet
(
	iudmd_t dict,
	char * key
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUDM_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUDM_INVALID);
    HashableDemodulation * search;
	search = HashableDemodulation_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IUDM_INVALID;
    }
	return search->demodulation;
}

char **iufDemodulationDictGetKeys
(
    iudmd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufDemodulationDictUpdateKeys
(
    iudmd_t dict
)
{
    iufDemodulationDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufDemodulationDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableDemodulation *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufDemodulationDictSet
(
	iudmd_t dict,
	char * key,
	iudm_t member
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    HashableDemodulation *newMember = calloc(1, sizeof(HashableDemodulation));
	newMember->demodulation = member;
	strcpy(newMember->key, key);
	if (HashableDemodulation_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUF_ERR_VALUE;
	}
	return iufDemodulationDictUpdateKeys(dict);
}

int iufDemodulationDictRemove
(
    iudmd_t dict,
    char * key
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    if (HashableDemodulation_hashmap_remove(&dict->map, key)==NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "key: %s does not exist in dictionary", key);
        return IUF_ERR_VALUE;
    }
    return iufDemodulationDictUpdateKeys(dict);
}

// serialization
int iufDemodulationDictSave
(
	iudmd_t dict,
	hid_t handle
)
{
	int status = 0;
	struct hashmap_iter *iter;
	hid_t group_id;

    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	status = H5Gget_objinfo(handle, IUF_PATH_DEMODULATIONDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_PATH_DEMODULATIONDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_PATH_DEMODULATIONDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_PATH_DEMODULATIONDICT);
        return IUF_ERR_VALUE;
    }

    status = 0;
	HashableDemodulation *demodulationDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		demodulationDictItem = HashableDemodulation_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, demodulationDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iufDemodulationSave(demodulationDictItem->demodulation, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}


iudmd_t iufDemodulationDictLoad
(
	hid_t handle
)
{
	int i;
	int status = 0;
	char member_name[IUF_MAX_HDF5_PATH];

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDMD_INVALID);
	hid_t grpid = H5Gopen(handle, IUF_PATH_DEMODULATIONDICT, H5P_DEFAULT);
	if (handle == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iudmd_t dict = iufDemodulationDictCreate();
	for (i = 0; i < (int)nobj; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, member_name, (size_t)IUF_MAX_HDF5_PATH);

		hid_t settings_id = H5Gopen(grpid, member_name, H5P_DEFAULT);
		iudm_t demodulation = iufDemodulationLoad(settings_id);
		status = iufDemodulationDictSet(dict, member_name, demodulation);
		H5Gclose(settings_id);
	}
	H5Gclose(grpid);

	if (status != IUF_E_OK)
	{
		return NULL;
	}
	dict->deepDelete = IUF_TRUE;
	return dict;
}
