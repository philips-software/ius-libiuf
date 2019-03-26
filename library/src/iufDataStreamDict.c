//
// Created by nlv12901 on 10/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufDataStream.h>
#include <iufDataStreamDict.h>
#include <iufDataStreamDictADT.h>

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableDataStream, const char, struct HashableDataStream)

// ADT
iudsd_t iufDataStreamDictCreate
(
	void
)
{
	iudsd_t dict = calloc(1, sizeof(IufDataStreamDict));
	IUF_ERR_ALLOC_NULL_N_RETURN(dict, IufDataStreamDict, IUDSD_INVALID);
	hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	return dict;
}

static void iufDataStreamDictDeleteKeys
(
    iudsd_t dict
)
{
    if (dict->kys != NULL)
        free(dict->kys);
}

int iufDataStreamDictDelete
(
	iudsd_t dict
)
{
    HashableDataStream *iterElement;
    struct hashmap_iter *iter;

	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashableDataStream_hashmap_iter_get_data(iter);
        iufDataStreamDelete(iterElement->dataStream);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
    iufDataStreamDictDeleteKeys(dict);
	free(dict);
	return IUF_E_OK;
}

static int iufDataStreamDictSourceInTarget
(
	iudsd_t source,
	iudsd_t target
)
{
	iuds_t targetElement;
	iuds_t sourceElement;
	HashableDataStream *iterElement;

	IUF_BOOL sourceInTarget = IUF_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashableDataStream_hashmap_iter_get_data(iter);
		sourceElement = iterElement->dataStream;
		targetElement = iufDataStreamDictGet(target, iterElement->key);
		if (targetElement == IUDS_INVALID)
			return IUF_FALSE;

		if (sourceElement->rfDataset != targetElement->rfDataset ||
		    sourceElement->fileChunkConfig != targetElement->fileChunkConfig)
			return IUF_FALSE;
		sourceInTarget = IUF_TRUE;
	}

	return sourceInTarget;
}

// operations
int iufDataStreamDictCompare
(
	iudsd_t reference,
	iudsd_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUF_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUF_TRUE;

	// check if elements of source hash are in target hash
	if (iufDataStreamDictSourceInTarget(reference, actual) == IUF_FALSE)
		return IUF_FALSE;

	// check if elements of target hash are in source hash
	return iufDataStreamDictSourceInTarget(actual, reference);
}


size_t iufDataStreamDictGetSize
(
	iudsd_t dict
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, -1);
	return (int)hashmap_size(&dict->map);
}

iuds_t iufDataStreamDictGet
(
	iudsd_t dict,
	char * key
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dict, IUDS_INVALID);
	IUF_ERR_CHECK_NULL_N_RETURN(key, IUDS_INVALID);
	HashableDataStream * search;
	search = HashableDataStream_hashmap_get(&dict->map, key);
	if (search == NULL)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_INVALID_KEY, "for key '%s'", key);
		return IUDS_INVALID;
	}
	return search->dataStream;
}

static int iufDataStreamDictUpdateKeys
(
    iudsd_t dict
)
{
    iufDataStreamDictDeleteKeys(dict);
    // allocate memory for the keys
    int keyIndex;
    size_t size = iufDataStreamDictGetSize(dict);
    dict->kys = calloc(size+1, sizeof(char*));
    IUF_ERR_ALLOC_NULL_N_RETURN(dict, char *, IUF_ERR_VALUE);

    struct hashmap_iter *iter;
    HashableDataStream *iterElement;
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    /* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map), keyIndex=0; iter; iter = hashmap_iter_next(&dict->map, iter), keyIndex++)
    {
        iterElement = HashableDataStream_hashmap_iter_get_data(iter);
        dict->kys[keyIndex] = iterElement->key;
    }
    dict->kys[keyIndex] = NULL;
    return IUF_E_OK;
}

int iufDataStreamDictSet
(
	iudsd_t dict,
	char * key,
	iuds_t member
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    HashableDataStream *newMember = calloc(1, sizeof(HashableDataStream));
	newMember->dataStream = member;
	strcpy(newMember->key, key);
	if (HashableDataStream_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "discarding blob with duplicate key: %s", key);
		free(newMember);
		return IUF_ERR_VALUE;
	}
	return iufDataStreamDictUpdateKeys(dict);
}

int iufDataStreamDictRemove
(
    iudsd_t dict,
    char * key
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(dict, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(key, IUF_ERR_VALUE);
    if (HashableDataStream_hashmap_remove(&dict->map, key)==NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_DUPLICATE_KEY, "key: %s does not exist in dictionary", key);
        return IUF_ERR_VALUE;
    }
    return iufDataStreamDictUpdateKeys(dict);
}

