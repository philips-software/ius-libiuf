
// Created by nlv09165 on 10/10/2018.
#ifndef IUSLIBRARY_IUSDATASTREAM_H
#define IUSLIBRARY_IUSDATASTREAM_H
#include <hdf5.h>

// ADT
typedef struct _IusDataStream
{
    hid_t fileChunkConfig;                /**< file chunck handle   */
    hid_t rfDataset;                      /**< dataset handle       */
} IusDataStream ;

typedef IusDataStream *iuds_t;
#define  IUDS_INVALID (iuds_t) NULL

iuds_t iusDataStreamCreate
(

);

int iusDataStreamDelete
(
    iuds_t iusDataStream
);



#endif //IUSLIBRARY_IUSDATASTREAM_H
