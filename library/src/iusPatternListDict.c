//
// Created by nlv12901 on 08/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusPatternListPrivate.h>

// ADT
struct HashablePatternList
{
	iupal_t patternList;
	char key[256];
};

typedef struct HashablePatternList HashablePatternList;

struct IusPatternListDict
{
	struct hashmap map;
	IUS_BOOL loadedFromFile;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashablePatternList, const char, struct HashablePatternList)

// ADT
iupald_t iusPatternListDictCreate
(
)
{
	iupald_t dict = calloc(1, sizeof(IusPatternListDict));
	if (dict != NULL)
	{
		hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
		dict->loadedFromFile = IUS_FALSE;
	}
	return dict;
}

int iusPatternListDictDelete
(
	iupald_t dict
)
{
    HashablePatternList *iterElement;
    struct hashmap_iter *iter;
	if (dict == NULL) return IUS_ERR_VALUE;
	/* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashablePatternList_hashmap_iter_get_data(iter);
        if (dict->loadedFromFile == IUS_TRUE)
            iusPatternListDelete(iterElement->patternList);
        free(iterElement);
    }
	hashmap_destroy(&dict->map);
	free(dict);
	return IUS_E_OK;
}


static int iusPatternListDictSourceInTarget
(
	iupald_t source,
	iupald_t target
)
{
	iupal_t targetElement;
	iupal_t sourceElement;
	HashablePatternList *iterElement;

	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashablePatternList_hashmap_iter_get_data(iter);
		sourceElement = iterElement->patternList;
		targetElement = iusPatternListDictGet(target, iterElement->key);
		if (targetElement == IUPAL_INVALID)
			return IUS_FALSE;

		if (iusPatternListCompare(sourceElement, targetElement) == IUS_FALSE)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusPatternListDictCompare
(
	iupald_t reference,
	iupald_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusPatternListDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusPatternListDictSourceInTarget(actual, reference);
}


int iusPatternListDictGetSize
(
	iupald_t dict
)
{
	if (dict == NULL)
		return -1;
	return (int)hashmap_size(&dict->map);
}

iupal_t iusPatternListDictGet
(
	iupald_t dict,
	char * key
)
{
	if (dict == NULL || key == NULL) return IUPAL_INVALID;
	HashablePatternList * search;
	search = HashablePatternList_hashmap_get(&dict->map, key);
	if (search == NULL)
		return IUPAL_INVALID;
	return search->patternList;
}

int iusPatternListDictSet
(
	iupald_t dict,
	char * key,
	iupal_t member
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	if (key == NULL) return IUS_ERR_VALUE;

	HashablePatternList *newMember = calloc(1, sizeof(HashablePatternList));
	newMember->patternList = member;
	strcpy(newMember->key, key);
	if (HashablePatternList_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
}


// serialization
int iusPatternListDictSave
(
	iupald_t dict,
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
	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	}
	if (group_id == H5I_INVALID_HID)
		return IUS_ERR_VALUE;
	status = 0;
	HashablePatternList *patternListDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == IUS_E_OK; iter = hashmap_iter_next(&dict->map, iter))
	{
		patternListDictItem = HashablePatternList_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, patternListDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		iusPatternListSave(patternListDictItem->patternList, subgroup_id);
		status |= H5Gclose(subgroup_id);
	}
	status |= H5Gclose(group_id);
	return status;
}


#define MAX_NAME 1024

iupald_t iusPatternListDictLoad
(
	hid_t handle
)
{
    hsize_t i;
	int status;
	char memb_name[MAX_NAME];

	hid_t grpid = H5Gopen(handle, IUS_INPUTFILE_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	if (handle == H5I_INVALID_HID || grpid == H5I_INVALID_HID)
		return NULL;

	hsize_t nobj;
	status = H5Gget_num_objs(grpid, &nobj);

	iupald_t dict = iusPatternListDictCreate();
	for (i = 0; i < nobj && status == IUS_E_OK; i++)
	{
		H5Gget_objname_by_idx(grpid, (hsize_t)i, memb_name, (size_t)MAX_NAME);
		hid_t patternList_id = H5Gopen(grpid, memb_name, H5P_DEFAULT);
		iupal_t patternList = iusPatternListLoad(patternList_id); 
		status |= H5Gclose(patternList_id);
		status |= iusPatternListDictSet(dict, memb_name, patternList);
	}
	H5Gclose(grpid);
	if (status != IUS_E_OK)
	{
		return NULL;
	}
	dict->loadedFromFile = IUS_TRUE;
	return dict;
}
