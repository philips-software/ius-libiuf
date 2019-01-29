//
// Created by nlv12901 on 28/01/2019.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusReceiveApodizationPrivate.h>

// ADT
struct HashableReceiveApodization
{
	iura_t receiveApodization;
	char key[256];
};

typedef struct HashableReceiveApodization HashableReceiveApodization;

struct IusReceiveApodizationDict
{
	struct hashmap map;
    IUS_BOOL loadedFromFile;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableReceiveApodization, const char, struct HashableReceiveApodization)

// ADT
iurad_t iusReceiveApodizationDictCreate()
{
	iurad_t dict = calloc(1, sizeof(IusReceiveApodizationDict));
	if (dict != NULL)
	{
		hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
		dict->loadedFromFile = IUS_FALSE;
	}
	return dict;
}


int iusReceiveApodizationDictDeepDelete
(
	iurad_t dict
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	dict->loadedFromFile = IUS_TRUE;
	return iusReceiveApodizationDictDelete(dict);
}

int iusReceiveApodizationDictDelete
(
	iurad_t dict
)
{
	struct hashmap_iter *iter;
	HashableReceiveApodization *iterElement;
	if (dict == NULL) return IUS_ERR_VALUE;
	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableReceiveApodization_hashmap_iter_get_data(iter);
		if(dict->loadedFromFile==IUS_TRUE)
		    iusReceiveApodizationDelete(iterElement->receiveApodization);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
	free(dict);
	return IUS_E_OK;
}

static int iusReceiveApodizationDictSourceInTarget
(
	iurad_t source,
	iurad_t target
)
{
	HashableReceiveApodization *targetElement;
	HashableReceiveApodization *sourceElement;
	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		sourceElement = HashableReceiveApodization_hashmap_iter_get_data(iter);
		targetElement = HashableReceiveApodization_hashmap_get(&target->map, sourceElement->key);
		if (targetElement == NULL)
			return IUS_FALSE;

		if (iusReceiveApodizationCompare(sourceElement->receiveApodization, targetElement->receiveApodization) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusReceiveApodizationDictCompare
(
	iurad_t reference,
	iurad_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusReceiveApodizationDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusReceiveApodizationDictSourceInTarget(actual, reference);
}


size_t iusReceiveApodizationDictGetSize
(
	iurad_t dict
)
{
	if (dict == NULL) return 0;
	return hashmap_size(&dict->map);
}

iura_t iusReceiveApodizationDictGet
(
	iurad_t dict,
	char * key
)
{
	if (dict == NULL || key == NULL) return NULL;
	HashableReceiveApodization *search;

	search = HashableReceiveApodization_hashmap_get(&dict->map, key);
	if (search == NULL) return NULL;
	return search->receiveApodization;
}

int iusReceiveApodizationDictSet
(
	iurad_t dict,
	char * key,
	iura_t member
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	if (key == NULL) return IUS_ERR_VALUE;

	HashableReceiveApodization *newMember = calloc(1, sizeof(HashableReceiveApodization));
	newMember->receiveApodization = member;
	strcpy(newMember->key, key);
	if (HashableReceiveApodization_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
}

herr_t iusReceiveApodizationDictSave
(
	iurad_t dict,
	hid_t handle
)
{
	herr_t status = 0;
	hid_t group_id;
	struct hashmap_iter *iter;

	if (dict == NULL)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

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
		return IUS_ERR_VALUE;
	status = 0;
	HashableReceiveApodization *receiveApodizationDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == 0; iter = hashmap_iter_next(&dict->map, iter))
	{
		receiveApodizationDictItem = HashableReceiveApodization_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, receiveApodizationDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusReceiveApodizationSave(receiveApodizationDictItem->receiveApodization, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}

	status |= H5Gclose(group_id);
	return status;
}

#define MAX_NAME 1024

iurad_t iusReceiveApodizationDictLoad
(
	hid_t handle
)
{
	int status = 0;
	iura_t receiveApodization;
	hsize_t i;
	char memberName[MAX_NAME];

	hid_t groupId = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT);
	if (handle == H5I_INVALID_HID || groupId == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
	status = H5Gget_num_objs(groupId, &nobj);

	iurad_t dict = iusReceiveApodizationDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(groupId, i, memberName, (size_t)MAX_NAME);
		hid_t subgroupId = H5Gopen(groupId, memberName, H5P_DEFAULT);
		receiveApodization = iusReceiveApodizationLoad(subgroupId);
		status |= iusReceiveApodizationDictSet(dict, memberName, receiveApodization);
		H5Gclose(subgroupId);
	}
	H5Gclose(groupId);

	if (status != IUS_E_OK)
	{
		return NULL;
	}
    dict->loadedFromFile = IUS_TRUE;
	return dict;
}

