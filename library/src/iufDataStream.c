
#include <stdlib.h>

#include <iuf.h>

// ADT
iuds_t iufDataStreamCreate
(
    void
)
{
    iuds_t created = calloc(1,sizeof(IufDataStream));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufDataStream, IUDS_INVALID);
    created->rfDataset = H5I_INVALID_HID;
    created->fileChunkConfig = H5I_INVALID_HID;
    return created;
}

int iufDataStreamDelete
(
    iuds_t dataStream
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(dataStream, IUF_ERR_VALUE);
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
