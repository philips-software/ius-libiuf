
//
// Created by nlv09165 on 10/10/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusDataStream.h"

// ADT
iuds_t iusDataStreamCreate
(
    void
)
{
    iuds_t created = calloc(1,sizeof(IusDataStream));
    created->rfDataset = H5I_INVALID_HID;
    created->fileChunkConfig = H5I_INVALID_HID;
    return created;
}

int iusDataStreamDelete
(
    iuds_t iusDataStream
)
{
    int status=0;
    if (iusDataStream == NULL) return IUS_ERR_VALUE;
    if (iusDataStream->fileChunkConfig != H5I_INVALID_HID)
    {
        status |= H5Pclose(iusDataStream->fileChunkConfig);
    }

    if (iusDataStream->rfDataset != H5I_INVALID_HID)
    {
        status |= H5Dclose(iusDataStream->rfDataset);
    }
    free(iusDataStream);
    return status;
}
