//
// Created by nlv12901 on 10/10/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusDataStreamDictPrivate.h>

// ADT
struct HashableDataStream
{
	iuds_t dataStream;
	char key[256];
};

typedef struct HashableDataStream HashableDataStream;

struct IusDataStreamDict
{
	struct hashmap map;
};

/* Declare type-specific blob_hashmap_* functions with this handy macro */
HASHMAP_FUNCS_CREATE(HashableDataStream, const char, struct HashableDataStream)

// ADT
iudsd_t iusDataStreamDictCreate
(
)
{
	iudsd_t dict = calloc(1, sizeof(IusDataStreamDict));
	if (dict != NULL)
	{
		hashmap_init(&dict->map, hashmap_hash_string, hashmap_compare_string, 0);
	}
	return dict;
}

int iusDataStreamDictDelete
(
	iudsd_t dict
)
{
    HashableDataStream *iterElement;
    struct hashmap_iter *iter;
	if (dict == NULL) return IUS_ERR_VALUE;
	/* Free all allocated resources associated with map and reset its state */
    for (iter = hashmap_iter(&dict->map); iter; iter = hashmap_iter_next(&dict->map, iter))
    {
        iterElement = HashableDataStream_hashmap_iter_get_data(iter);
        iusDataStreamDelete(iterElement->dataStream);
        free(iterElement);
    }
    hashmap_destroy(&dict->map);
	free(dict);
	return IUS_E_OK;
}


static int iusDataStreamDictSourceInTarget
(
	iudsd_t source,
	iudsd_t target
)
{
	iuds_t targetElement;
	iuds_t sourceElement;
	HashableDataStream *iterElement;

	IUS_BOOL sourceInTarget = IUS_FALSE;
	struct hashmap_iter *iter;

	// iterate over source list elements using the hash double linked list
	for (iter = hashmap_iter(&source->map); iter; iter = hashmap_iter_next(&source->map, iter)) {
		iterElement = HashableDataStream_hashmap_iter_get_data(iter);
		sourceElement = iterElement->dataStream;
		targetElement = iusDataStreamDictGet(target, iterElement->key);
		if (targetElement == IUDS_INVALID)
			return IUS_FALSE;

		if (sourceElement->rfDataset != targetElement->rfDataset ||
		    sourceElement->fileChunkConfig != targetElement->fileChunkConfig)
			return IUS_FALSE;
		sourceInTarget = IUS_TRUE;
	}

	return sourceInTarget;
}

// operations
int iusDataStreamDictCompare
(
	iudsd_t reference,
	iudsd_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (hashmap_size(&reference->map) != hashmap_size(&actual->map)) return IUS_FALSE;
	if (hashmap_size(&reference->map) == 0) return IUS_TRUE;

	// check if elements of source hash are in target hash
	if (iusDataStreamDictSourceInTarget(reference, actual) == IUS_FALSE)
		return IUS_FALSE;

	// check if elements of target hash are in source hash
	return iusDataStreamDictSourceInTarget(actual, reference);
}


int iusDataStreamDictGetSize
(
	iudsd_t dict
)
{
	if (dict == NULL)
		return -1;
	return (int)hashmap_size(&dict->map);
}

iuds_t iusDataStreamDictGet
(
	iudsd_t dict,
	char * key
)
{
	if (dict == NULL || key == NULL) return IUDS_INVALID;
	HashableDataStream * search;
	search = HashableDataStream_hashmap_get(&dict->map, key);
	if (search == NULL)
		return IUDS_INVALID;
	return search->dataStream;
}

int iusDataStreamDictSet
(
	iudsd_t dict,
	char * key,
	iuds_t member
)
{
	if (dict == NULL) return IUS_ERR_VALUE;
	if (key == NULL) return IUS_ERR_VALUE;
    if (member == IUDS_INVALID) return IUS_ERR_VALUE;

	HashableDataStream *newMember = calloc(1, sizeof(HashableDataStream));
	newMember->dataStream = member;
	strcpy(newMember->key, key);
	if (HashableDataStream_hashmap_put(&dict->map, newMember->key, newMember) != newMember)
	{
		printf("discarding blob with duplicate key: %s\n", newMember->key);
		free(newMember);
		return IUS_ERR_VALUE;
	}
	return IUS_E_OK;
}

