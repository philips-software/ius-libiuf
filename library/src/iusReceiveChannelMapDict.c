//
// Created by nlv12901 on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusReceiveChannelMapPrivate.h>

// ADT
struct HashableReceiveChannelMap
{
	iurcm_t receiveChannelMap;
	char key[256];
};

typedef struct HashableReceiveChannelMap HashableReceiveChannelMap;

struct IusReceiveChannelMapDict
{
	struct hashmap map;
	IUS_BOOL deepDelete;
    char **keys;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveChannelMap, const char, struct HashableReceiveChannelMap)

// ADT
iurcmd_t iusReceiveChannelMapDictCreate
(
)
{
	iurcmd_t dict = calloc(1, sizeof(IusReceiveChannelMapDict));
	IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusReceiveChannelMapDict, IURCMD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUS_FALSE;
    dict->keys = NULL;
	return dict;
}

static void iusReceiveChannelMapDictDeleteKeys
        (
                iurcmd_t dict
        )
{
    if (dict->keys != NULL)
        free(dict->keys);
}

int iusReceiveChannelMapDictDelete
(
	iurcmd_t dict
)
{
	HashableReceiveChannelMap *iterElement;
	struct hashmap_iter *iter;
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
		if(dict->deepDelete==IUS_TRUE)
			iusReceiveChannelMapDelete(iterElement->receiveChannelMap);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
    iusReceiveChannelMapDictDeleteKeys(dict);
	free(dict);
	return IUS_E_OK;
}

int iusReceiveChannelMapDictDeepDelete
(
	iurcmd_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	dict->deepDelete=IUS_TRUE;
	return iusReceiveChannelMapDictDelete(dict);
}

static int iusReceiveChannelMapDictSourceInTarget
(
	iurcmd_t source,
	iurcmd_t target
)
{
	HashableReceiveChannelMap *targetElement;
	HashableReceiveChannelMap *sourceElement;
	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		sourceElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
		targetElement = HashableReceiveChannelMap_hashmap_get(&target->map, sourceElement->key);
		if (targetElement == NULL)
			return IUS_FALSE;

		if (iusReceiveChannelMapCompare(sourceElement->receiveChannelMap, targetElement->receiveChannelMap) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusReceiveChannelMapDictCompare
(
	iurcmd_t reference,
	iurcmd_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusReceiveChannelMapDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusReceiveChannelMapDictSourceInTarget(actual, reference);
}


size_t iusReceiveChannelMapDictGetSize
(
	iurcmd_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, (size_t) -1);
	return hashmap_size(&dict->map);
}

iurcm_t iusReceiveChannelMapDictGet
(
	iurcmd_t dict,
	char * key
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IURCM_INVALID);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IURCM_INVALID);
	HashableReceiveChannelMap *search;
	search = HashableReceiveChannelMap_hashmap_get(&dict->map, key);
    if (search == NULL)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
        return IURCM_INVALID;
    }
	return search->receiveChannelMap;
}

char **iusReceiveChannelMapDictGetKeys
(
    iurcmd_t dict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
    return dict->keys;
}

static int iusReceiveSettingsUpdateKeys
(
    iurcmd_t dict
)
{
    iusReceiveChannelMapDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iusReceiveChannelMapDictGetSize(dict);
    dict->keys = calloc(size+1, sizeof(char*));
    IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableReceiveChannelMap *iterElement;
    IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
        dict->keys[keyIndex] = iterElement->key;
    }
    dict->keys[keyIndex] = NULL;
    return IUS_E_OK;
}

int iusReceiveChannelMapDictSet
(
	iurcmd_t dict,
	char * key,
	iurcm_t member
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);
	HashableReceiveChannelMap *newMember = calloc(1, sizeof(HashableReceiveChannelMap));
	newMember->receiveChannelMap = member;
	strcpy(newMember->key, key);
	if (HashableReceiveChannelMap_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return iusReceiveSettingsUpdateKeys(dict);
}

herr_t iusReceiveChannelMapDictSave
(
	iurcmd_t dict,
	hid_t handle
)
{
	herr_t status = 0;
	struct hashmap_iter *iter;
	hid_t group_id;

	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, H5P_DEFAULT);
	}

    if (group_id == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT);
        return IUS_ERR_VALUE;
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
			return IUS_ERR_VALUE;
		}
		
		status |= iusReceiveChannelMapSave(receiveChannelMapDictItem->receiveChannelMap, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}

#define MAX_NAME 1024

iurcmd_t iusReceiveChannelMapDictLoad
(
	hid_t handle
)
{
	int status = 0;
	iurcm_t receiveChannelMap;
	hsize_t i;
	char memberName[MAX_NAME];
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IURCMD_INVALID);
	hid_t groupId = H5Gopen(handle, IUS_INPUTFILE_PATH_RECEIVECHANNELMAPDICT, H5P_DEFAULT);
    if (groupId == H5I_INVALID_HID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_INPUTFILE_PATH_RECEIVECHANNELMAPDICT);
        return IURCMD_INVALID;
    }

	hsize_t nobj;
	status = H5Gget_num_objs(groupId, &nobj);

	iurcmd_t dict = iusReceiveChannelMapDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(groupId, i, memberName, (size_t)MAX_NAME);
		hid_t subgroupId = H5Gopen(groupId, memberName, H5P_DEFAULT);
		receiveChannelMap = iusReceiveChannelMapLoad(subgroupId);
		status = iusReceiveChannelMapDictSet(dict, memberName, receiveChannelMap);
		H5Gclose(subgroupId);
	}
	H5Gclose(handle);
	
	if (status != IUS_E_OK)
	{
		return IURCMD_INVALID;
	}
	dict->deepDelete = IUS_TRUE;
	return dict;
}

