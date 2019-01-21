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
	IUS_BOOL loadedFromFile;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableDemodulation, const char, struct HashableDemodulation)

// ADT
iudmd_t iusDemodulationDictCreate
(
)
{
	iudmd_t dict = calloc(1, sizeof(IusDemodulationDict));
	if (dict != NULL)
	{
		hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
		dict->loadedFromFile = IUS_FALSE;
	}
	return dict;
}

int iusDemodulationDictDeepDelete
(
	iudmd_t dict
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	dict->loadedFromFile = IUS_TRUE;
	return iusDemodulationDictDelete(dict);
}

int iusDemodulationDictDelete
(
	iudmd_t dict
)
{
	HashableDemodulation *iterElement;
	struct hashmap_iter *iter;

	if (dict == NULL) return IUS_ERR_VALUE;
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableDemodulation_hashmap_iter_get_data(iter);
		if (dict->loadedFromFile == IUS_TRUE)
			iusDemodulationDelete(iterElement->demodulation);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
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


int iusDemodulationDictGetSize
(
	iudmd_t dict
)
{
	if (dict == NULL)
		return -1;
	return (int)hashmap_size(&dict->map);
}

iudm_t iusDemodulationDictGet
(
	iudmd_t dict,
	char * key
)
{
	if (dict == NULL || key == NULL) return IUDM_INVALID;
	HashableDemodulation * search;
	search = HashableDemodulation_hashmap_get(&dict->map, key);
	if (search == NULL)
		return IUDM_INVALID;
	return search->demodulation;
}

int iusDemodulationDictSet
(
	iudmd_t dict,
	char * key,
	iudm_t member
)
{
	if (dict == NULL || key == NULL) return IUS_ERR_VALUE;
	HashableDemodulation *newMember = calloc(1, sizeof(HashableDemodulation));
	newMember->demodulation = member;
	strcpy(newMember->key, key);
	if (HashableDemodulation_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
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

	if (dict == NULL)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

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
		return IUS_ERR_VALUE;
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
	dict->loadedFromFile = IUS_TRUE;
	return dict;
}
