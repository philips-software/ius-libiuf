//
// Created by nlv12901 on 18/07/2018.
//
#include <stdlib.h>
#include <math.h>

#include <hashmap.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLReceiveChannelMapImp.h>
#include <iusHLReceiveChannelMapDict.h>
#include <assert.h>
#include <string.h>
#include <include/iusHDF5.h>

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
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveChannelMap, const char, struct HashableReceiveChannelMap)

// ADT
iurcmd_t iusHLReceiveChannelMapDictCreate
(
)
{
	iurcmd_t list = calloc(1, sizeof(IusReceiveChannelMapDict));
	if (list != NULL)
	{
		hashmap_init(&list->map, hashmap_hash_string, hashmap_compare_string, 0);
	}
	return list;
}

int iusHLReceiveChannelMapDictDelete
(
	iurcmd_t dict
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	/* Free all allocated resources associated with map and reset its state */
	hashmap_destroy(&dict->map);
	free(dict);
	return IUS_E_OK;
}

static int iusHLReceiveChannelMapDictSourceInTarget
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

		if (iusHLReceiveChannelMapCompare(sourceElement->receiveChannelMap, targetElement->receiveChannelMap) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusHLReceiveChannelMapDictCompare
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
	if (iusHLReceiveChannelMapDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusHLReceiveChannelMapDictSourceInTarget(actual, reference);
}


size_t iusHLReceiveChannelMapDictGetSize
(
	iurcmd_t dict
)
{
	if (dict == NULL) return -1;
	return hashmap_size(&dict->map);
}

iurcm_t iusHLReceiveChannelMapDictGet
(
	iurcmd_t dict,
	char * key
)
{
	HashableReceiveChannelMap *search;
	if (dict == NULL) return IURCM_INVALID;

	search = HashableReceiveChannelMap_hashmap_get(&dict->map, key);
	if (search == NULL) return IURCM_INVALID;

	return search->receiveChannelMap;
}

int iusHLReceiveChannelMapDictSet
(
	iurcmd_t dict,
	char * key,
	iurcm_t member
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	if (key == NULL) return IUS_ERR_VALUE;

	HashableReceiveChannelMap *newMember = calloc(1, sizeof(HashableReceiveChannelMap));
	newMember->receiveChannelMap = member;
	strcpy(newMember->key, key);
	if (HashableReceiveChannelMap_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
}

herr_t iusHLReceiveChannelMapDictSave
(
	iurcmd_t dict,
	char *parentPath,
	hid_t handle
)
{
	herr_t status = 0;
	char path[IUS_MAX_HDF5_PATH];
	struct hashmap_iter *iter;

	if (dict == NULL)
		return IUS_ERR_VALUE;
	if (parentPath == NULL || handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

	hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	HashableReceiveChannelMap *sourceElement;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		hid_t subgroup_id;
		sourceElement = HashableReceiveChannelMap_hashmap_iter_get_data(iter);
		sprintf(path, "%s/%s", parentPath, sourceElement->key);
		subgroup_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusHLReceiveChannelMapSave(sourceElement->receiveChannelMap, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}

#define MAX_NAME 1024

iurcmd_t iusHLReceiveChannelMapDictLoad
(
	hid_t handle,
	char *parentPath
)
{
	int status = 0;
	//char path[64];
	iurcm_t receiveChannelMap;
	hsize_t i;
	char memberName[MAX_NAME];

	hid_t groupId = H5Gopen(handle, parentPath, H5P_DEFAULT);
	if (parentPath == NULL || handle == H5I_INVALID_HID || groupId == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
	status = H5Gget_num_objs(groupId, &nobj);

	iurcmd_t dict = iusHLReceiveChannelMapDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(groupId, i, memberName, (size_t)MAX_NAME);
		//sprintf(path, "%s/%s", parentPath, memberName);
		hid_t subgroupId = H5Gopen(groupId, memberName, H5P_DEFAULT);
		receiveChannelMap = iusHLReceiveChannelMapLoad(subgroupId);
		status = iusHLReceiveChannelMapDictSet(dict, memberName, receiveChannelMap);
		//iusHLReceiveChannelMapDelete(receiveChannelMap); do not delete becasue then it is out of the dictionary?
		H5Gclose(subgroupId);
	}
	H5Gclose(handle);
	
	if (status != IUS_E_OK)
	{
		return NULL;
	}

	return dict;
}

