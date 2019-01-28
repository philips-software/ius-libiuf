//
// Created by nlv12901 on 08/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusIqPatternListPrivate.h>

// ADT
struct HashableIqPatternList
{
	iuiqpal_t patternList;
	char key[256];
};

typedef struct HashableIqPatternList HashableIqPatternList;

struct IusIqPatternListDict
{
	struct hashmap map;
	IUS_BOOL loadedFromFile;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableIqPatternList, const char, struct HashableIqPatternList)

// ADT
iuiqpald_t iusIqPatternListDictCreate
(
)
{
	iuiqpald_t dict = calloc(1, sizeof(IusIqPatternListDict));
	if (dict != NULL)
	{
		hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
		dict->loadedFromFile = IUS_FALSE;
	}
	return dict;
}



int iusIqPatternListDictDeepDelete
(
    iuiqpald_t dict
)
{
    if (dict == NULL) return IUS_ERR_VALUE;
    dict->loadedFromFile = IUS_TRUE;
    return iusIqPatternListDictDelete(dict);
}

int iusIqPatternListDictDelete
(
	iuiqpald_t dict
)
{
    HashableIqPatternList *iterElement;
    struct hashmap_iter *iter;
	if (dict == NULL) return IUS_ERR_VALUE;
	/* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
        if (dict->loadedFromFile == IUS_TRUE)
            iusIqPatternListDeepDelete(iterElement->patternList);
        free(iterElement);
    }
	hashmap_destroy(&dict->map);
	free(dict);
	return IUS_E_OK;
}


static int iusIqPatternListDictSourceInTarget
(
	iuiqpald_t source,
	iuiqpald_t target
)
{
	iuiqpal_t targetElement;
	iuiqpal_t sourceElement;
	HashableIqPatternList *iterElement;

	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
		sourceElement = iterElement->patternList;
		targetElement = iusIqPatternListDictGet(target, iterElement->key);
		if (targetElement == IUIQPAL_INVALID)
			return IUS_FALSE;

		if (iusIqPatternListCompare(sourceElement, targetElement) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusIqPatternListDictCompare
(
	iuiqpald_t reference,
	iuiqpald_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusIqPatternListDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusIqPatternListDictSourceInTarget(actual, reference);
}


int iusIqPatternListDictGetSize
(
	iuiqpald_t dict
)
{
	if (dict == NULL)
		return -1;
	return (int)hashmap_size(&dict->map);
}

iuiqpal_t iusIqPatternListDictGet
(
	iuiqpald_t dict,
	char * key
)
{
	if (dict == NULL || key == NULL) return IUIQPAL_INVALID;
	HashableIqPatternList * search;
	search = HashableIqPatternList_hashmap_get(&dict->map, key);
	if (search == NULL)
		return IUIQPAL_INVALID;
	return search->patternList;
}

int iusIqPatternListDictSet
(
	iuiqpald_t dict,
	char * key,
	iuiqpal_t member
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	if (key == NULL) return IUS_ERR_VALUE;

	HashableIqPatternList *newMember = calloc(1, sizeof(HashableIqPatternList));
	newMember->patternList = member;
	strcpy(newMember->key, key);
	if (HashableIqPatternList_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
}


// serialization
int iusIqPatternListDictSave
(
	iuiqpald_t dict,
	hid_t handle
)
{
	int status = 0;
	struct hashmap_iter *iter;

	if (dict == NULL)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;
	hid_t group_id;
	status = H5Gget_objinfo(handle, IUS_IQFILE_PATH_PATTERNLISTDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_IQFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_IQFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	}
	if (group_id == H5I_INVALID_HID)
		return IUS_ERR_VALUE;
	status = 0;
	HashableIqPatternList *patternListDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == IUS_E_OK; iter = hashmap_iter_next(&dict->map, iter))
	{
		patternListDictItem = HashableIqPatternList_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, patternListDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		iusIqPatternListSave(patternListDictItem->patternList, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}
	status |= H5Gclose(group_id);
	return status;
}


#define MAX_NAME 1024

iuiqpald_t iusIqPatternListDictLoad
(
	hid_t handle
)
{
    hsize_t i;
	int status;
	char memb_name[MAX_NAME];

	hid_t grpid = H5Gopen(handle, IUS_IQFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	if (handle == H5I_INVALID_HID || grpid == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iuiqpald_t dict = iusIqPatternListDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, memb_name, (size_t)MAX_NAME);
		hid_t patternList_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
		iuiqpal_t patternList = iusIqPatternListLoad(patternList_id); 
		status |= H5Gclose(patternList_id);
		status |= iusIqPatternListDictSet(dict, memb_name, patternList);
	}
	H5Gclose(grpid);
	if (status != IUS_E_OK)
	{
		return NULL;
	}
	dict->loadedFromFile = IUS_TRUE;
	return dict;
}
