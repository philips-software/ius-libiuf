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
	iuiqpal_t iqPatternList;
	char key[256];
};

typedef struct HashableIqPatternList HashableIqPatternList;

struct IusIqPatternListDict
{
	struct hashmap map;
	IUS_BOOL deepDelete;
	char **keys;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableIqPatternList, const char, struct HashableIqPatternList)

// ADT
iuiqpald_t iusIqPatternListDictCreate
(
)
{
	iuiqpald_t dict = calloc(1, sizeof(IusIqPatternListDict));
	IUS_ERR_ALLOC_NULL_N_RETURN(dict, IusIqPatternListDict, IUIQPALD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	dict->deepDelete = IUS_FALSE;
	dict->keys = NULL;
	return dict;
}



int iusIqPatternListDictDeepDelete
(
    iuiqpald_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	dict->deepDelete = IUS_TRUE;
	return iusIqPatternListDictDelete(dict);
}

static void iusIqPatternListDictDeleteKeys
(
	iuiqpald_t dict
)
{
	if (dict->keys != NULL)
		free(dict->keys);
}

int iusIqPatternListDictDelete
(
	iuiqpald_t dict
)
{
	HashableIqPatternList *iterElement;
	struct hashmap_iter *iter;
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);

	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
	{
		iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
		if (dict->deepDelete == IUS_TRUE)
			iusIqPatternListDeepDelete(iterElement->iqPatternList);
		free(iterElement);
	}
	hashmap_destroy(&dict->map);
	iusIqPatternListDictDeleteKeys(dict);
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
		sourceElement = iterElement->iqPatternList;
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


size_t iusIqPatternListDictGetSize
(
	iuiqpald_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, (size_t)-1);
	return (int)hashmap_size(&dict->map);
}

char **iusIqPatternListDictGetKeys
(
	iuiqpald_t dict
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, NULL);
	return dict->keys;
}

iuiqpal_t iusIqPatternListDictGet
(
	iuiqpald_t dict,
	char * key
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUIQPAL_INVALID);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUIQPAL_INVALID);
	HashableIqPatternList * search;
	search = HashableIqPatternList_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUIQPAL_INVALID;
	}
	return search->iqPatternList;
}

static int iusIqPatternListDictUpdateKeys
(
	iuiqpald_t dict
)
{
	iusIqPatternListDictDeleteKeys(dict);
	// allocate memory for the keys
	int keyIndex;
	size_t size = iusIqPatternListDictGetSize(dict);
	dict->keys = calloc(size + 1, sizeof(char*));
	IUS_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUS_ERR_VALUE);

	struct hashmap_iter *iter;
	HashableIqPatternList *iterElement;
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	/* Free all allocated resources associated with map and reset its state */
	for (iter = hashmap_iter(&dict->map), keyIndex = 0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
	{
		iterElement = HashableIqPatternList_hashmap_iter_get_data(iter);
		dict->keys[keyIndex] = iterElement->key;
	}
	dict->keys[keyIndex] = NULL;
	return IUS_E_OK;
}

int iusIqPatternListDictSet
(
	iuiqpald_t dict,
	char * key,
	iuiqpal_t member
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_CHECK_NULL_N_RETURN(key, IUS_ERR_VALUE);

	HashableIqPatternList *newMember = calloc(1, sizeof(HashableIqPatternList));
	newMember->iqPatternList = member;
	strcpy(newMember->key, key);
	if (HashableIqPatternList_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return iusIqPatternListDictUpdateKeys(dict);;
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

	IUS_ERR_CHECK_NULL_N_RETURN(dict, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	hid_t group_id;
	status = H5Gget_objinfo(handle, IUS_PATH_PATTERNLISTDICT, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_PATH_PATTERNLISTDICT, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	}
	if (group_id == H5I_INVALID_HID)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_PATH_PATTERNLISTDICT);
		return IUS_ERR_VALUE;
	}
	status = 0;
	HashableIqPatternList *iqPatternListDictItem;

	// iterate over source list elements and save'em
	for (iter = hashmap_iter(&dict->map); iter && status == IUS_E_OK; iter = hashmap_iter_next(&dict->map, iter))
	{
		iqPatternListDictItem = HashableIqPatternList_hashmap_iter_get_data(iter);
		hid_t subgroup_id = H5Gcreate(group_id, iqPatternListDictItem->key, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		if (subgroup_id <= 0)
		{
			H5Gclose(group_id);
			return IUS_ERR_VALUE;
		}
		iusIqPatternListSave(iqPatternListDictItem->iqPatternList, subgroup_id);
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

	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUIQPALD_INVALID);
	hid_t grpid = H5Gopen(handle, IUS_PATH_PATTERNLISTDICT, H5P_DEFAULT);
	if (grpid == H5I_INVALID_HID)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "Error getting handle for path: %s", IUS_PATH_PATTERNLISTDICT);
		return IUIQPALD_INVALID;
	}

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
		return IUIQPALD_INVALID;
	}
	dict->deepDelete = IUS_TRUE;
	return dict;
}
