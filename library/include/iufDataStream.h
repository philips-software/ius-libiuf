
#ifndef IUFLIBRARY_IUFDATASTREAM_H
#define IUFLIBRARY_IUFDATASTREAM_H
#include <hdf5.h>

// ADT
typedef struct _IufDataStream
{
    hid_t fileChunkConfig;                /**< file chunck handle   */
    hid_t rfDataset;                      /**< dataset handle       */
} IufDataStream ;

typedef IufDataStream *iuds_t;
#define  IUDS_INVALID (iuds_t) NULL

iuds_t iufDataStreamCreate
(

);

int iufDataStreamDelete
(
    iuds_t dataStream
);



#endif //IUFLIBRARY_IUFDATASTREAM_H
