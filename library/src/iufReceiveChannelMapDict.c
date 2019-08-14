#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufReceiveChannelMapDictADT.h>
#include <iufReceiveChannelMapPrivate.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveChannelMap, const char, struct HashableReceiveChannelMap)

// ADT
iurcmd_t iufReceiveChannelMapDictCreate
(
)
{
	iurcmd_t dict = calloc(1, sizeof(IufReceiveChannelMapDict));
	IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufReceiveChannelMapDict, IURCMD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUF_FALSE;
    dict->kys = NULL;
	return dict;
}

static void iufReceiveChannelMapDictDeleteKeys
(
    iurcmd_t dict
)
{
    if (dict->kys != NULL)
    {
        free(dict->kys);
    }
}

int iufReceiveChannelMapDictDelete
(
	iurcmd_t dict
)
{
	HashableReceiveChannelMap *iterElement;
	struct hashmap_iter *iter;
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
		if(dict->deepDelete==IUF_TRUE)
			iufReceiveChannelMapDelete(iterElement->receiveChannelMap);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
    iufReceiveChannelMapDictDeleteKeys(dict);
	free(dict);
	return IUF_E_OK;
}

int iufReceiveChannelMapDictDeepDelete
(
	iurcmd_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	dict->deepDelete=IUF_TRUE;
	return iufReceiveChannelMapDictDelete(dict);
}

static int iufReceiveChannelMapDictSourceInTarget
(
	iurcmd_t source,
	iurcmd_t target
)
{
	HashableReceiveChannelMap *targetElement;
	HashableReceiveChannelMap *sourceElement;
	IUF_BOOL sourceInTarget = IUF_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		sourceElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
		targetElement = HashableReceiveChannelMap_hashmap_get(&target->map, sourceElement->key);
		if (targetElement == NULL)
			return IUF_FALSE;

		if (iufReceiveChannelMapCompare(sourceElement->receiveChannelMap, targetElement->receiveChannelMap) == IUF_FALSE)
			return IUF_FALSE;
		sourceInTarget = IUF_TRUE;
	}

	return sourceInTarget;
}

// operations
int iufReceiveChannelMapDictCompare
(
	iurcmd_t reference,
	iurcmd_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUF_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUF_TRUE;

	// check if elements of source hash are in target hash
	if (iufReceiveChannelMapDictSourceInTarget(reference, actual) == IUF_FALSE)
		return IUF_FALSE;

	// check if elements of target hash are in source hash
	return iufReceiveChannelMapDictSourceInTarget(actual, reference);
}


size_t iufReceiveChannelMapDictGetSize
(
	iurcmd_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return hashmap_size(&dict->map);
}

iurcm_t iufReceiveChannelMapDictGet
(
	iurcmd_t dict,
	char * key
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IURCM_INVALID);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IURCM_INVALID);
	HashableReceiveChannelMap *search;
	search = HashableReceiveChannelMap_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IURCM_INVALID;
    }
	return search->receiveChannelMap;
}

char **iufReceiveChannelMapDictGetKeys
(
    iurcmd_t dict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->kys;
}

static int iufReceiveSettingsUpdateKeys
(
    iurcmd_t dict
)
{
    iufReceiveChannelMapDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufReceiveChannelMapDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableReceiveChannelMap *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufReceiveChannelMapDictSet
(
	iurcmd_t dict,
	char * key,
	iurcm_t member
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
	HashableReceiveChannelMap *newMember = calloc(1, sizeof(HashableReceiveChannelMap));
	newMember->receiveChannelMap = member;
	strncpy(newMember->key, key, sizeof(newMember->key));
	if (HashableReceiveChannelMap_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUF_ERR_VALUE;
	}
	return iufReceiveSettingsUpdateKeys(dict);
}

herr_t iufReceiveChannelMapDictSave
(
	iurcmd_t dict,
	hid_t handle
)
{
	herr_t status = 0;
	struct hashmap_iter *iter;
	hid_t group_id;

	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
	status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
        return IUF_ERR_VALUE;
    }

	status = 0;
	HashableReceiveChannelMap *receiveChannelMapDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		hid_t subgroup_id;
		receiveChannelMapDictItem = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
		subgroup_id = H5Gcreate(group_id, receiveChannelMapDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		if (subgroup_id <= 0)
		{
			H5Gclose(group_id);
			return IUF_ERR_VALUE;
		}
		
		status |= iufReceiveChannelMapSave(receiveChannelMapDictItem->receiveChannelMap, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}

#define MAX_NAME 1024

iurcmd_t iufReceiveChannelMapDictLoad
(
	hid_t handle
)
{
	int status = 0;
	iurcm_t receiveChannelMap;
	hsize_t i;
	char memberName[MAX_NAME];
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IURCMD_INVALID);
	hid_t groupId = H5Gopen(handle, IUF_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, H5P_DEFAULT);
    if (groupId == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_RECEIVECHANNELMAPDICT);
        return IURCMD_INVALID;
    }

	hsize_t nobj;
	status = H5Gget_num_objs(groupId, &nobj);

	iurcmd_t dict = iufReceiveChannelMapDictCreate();
	for (i = 0; i < nobj && status == IUF_E_OK; i++)
	{
		H5Gget_objname_by_idx(groupId, i, memberName, (size_t)MAX_NAME);
		hid_t subgroupId = H5Gopen(groupId, memberName, H5P_DEFAULT);
		receiveChannelMap = iufReceiveChannelMapLoad(subgroupId);
		status = iufReceiveChannelMapDictSet(dict, memberName, receiveChannelMap);
		H5Gclose(subgroupId);
	}
	H5Gclose(handle);
	
	if (status != IUF_E_OK)
	{
		return IURCMD_INVALID;
	}
	dict->deepDelete = IUF_TRUE;
	return dict;
}

