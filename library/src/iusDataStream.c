
//
// Created by nlv09165 on 10/10/2018.
//
#include <stdlib.h>

#include <ius.h>

// ADT
iuds_t iusDataStreamCreate
(
    void
)
{
    iuds_t created = calloc(1,sizeof(IusDataStream));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusDataStream, IUDS_INVALID);
    created->rfDataset = H5I_INVALID_HID;
    created->fileChunkConfig = H5I_INVALID_HID;
    return created;
}

int iusDataStreamDelete
(
    iuds_t dataStream
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(dataStream, IUS_ERR_VALUE);
    if (dataStream->fileChunkConfig != H5I_INVALID_HID)
    {
        status |= H5Pclose(dataStream->fileChunkConfig);
    }

    if (dataStream->rfDataset != H5I_INVALID_HID)
    {
        status |= H5Dclose(dataStream->rfDataset);
    }
    free(dataStream);
    return status;
}
