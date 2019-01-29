// Created by nlv09165 on 05/10/2018.
#ifndef IUSLIBRARY_IUSDATA_H
#define IUSLIBRARY_IUSDATA_H


// ADT
typedef struct IusData IusData;
/** A datablock is an array of floats.
 */
typedef IusData *iud_t;
#define  IUD_INVALID (iud_t) NULL

/** \brief Creates a datablock of \p size floats 
 * \return Returns the datablock or IUD_INVALID in case of a failure.
 */
iud_t iusDataCreate
(
    int size  ///< The number of floats to allocate
);

/** \brief Deletes a datablock of \p size floats 
 * \return Returns the datablock or IUD_INVALID in case of a failure.
 */
int iusDataDelete
(
    iud_t data
);


// operations
int iusDataCompare
(
    iud_t reference,
    iud_t actual
);

int iusDataGetSize
(
    iud_t data
);

float *iusDataGetPointer
(
    iud_t data
);

#endif //IUSLIBRARY_IUSDATA_H

