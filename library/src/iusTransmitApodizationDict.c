//
// Created by nlv12901 on 18/07/2018.
//
#include <stdlib.h>
#include <math.h>

#include <hashmap.h>
#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusInputFileStructure.h>
#include <iusTransmitApodizationPrivate.h>
#include <iusTransmitApodizationDict.h>
#include <assert.h>
#include <string.h>
#include <include/iusHDF5.h>

// ADT
struct HashableTransmitApodization
{
	iuta_t transmitApodization;
	char key[256];
};

typedef struct HashableTransmitApodization HashableTransmitApodization;

struct IusTransmitApodizationDict
{
	struct hashmap map;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableTransmitApodization, const char, struct HashableTransmitApodization)

// ADT
iutad_t iusTransmitApodizationDictCreate
(
)
{
	iutad_t list = calloc(1, sizeof(IusTransmitApodizationDict));
	if (list != NULL)
	{
		hashmap_init(&list->map, hashmap_hash_string, hashmap_compare_string, 0);
	}
	return list;
}

int iusTransmitApodizationDictDelete
(
	iutad_t dict
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	/* Free all allocated resources associated with map and reset its state */
	hashmap_destroy(&dict->map);
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
	if (dict == NULL) return 0;
	return hashmap_size(&dict->map);
}

iuta_t iusTransmitApodizationDictGet
(
	iutad_t dict,
	char * key
)
{
	HashableTransmitApodization *search;
	if (dict == NULL) return NULL;

	search = HashableTransmitApodization_hashmap_get(&dict->map, key);

	if (search == NULL) return NULL;
	return search->transmitApodization;
}

int iusTransmitApodizationDictSet
(
	iutad_t dict,
	char * key,
	iuta_t member
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	if (key == NULL) return IUS_ERR_VALUE;

	HashableTransmitApodization *newMember = calloc(1, sizeof(HashableTransmitApodization));
	newMember->transmitApodization = member;
	strcpy(newMember->key, key);
	if (HashableTransmitApodization_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
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

	if (dict == NULL)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, 0, NULL); // todo centralize the path
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

	hid_t groupId = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT, H5P_DEFAULT);
	if (handle == H5I_INVALID_HID || groupId == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
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
		return NULL;
	}

	return dict;
}

