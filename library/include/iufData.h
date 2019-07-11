#ifndef IUFLIBRARY_IUFDATA_H
#define IUFLIBRARY_IUFDATA_H


// ADT
typedef struct IufData IufData;
/** A datablock is an array of floats.
 */
typedef IufData *iud_t;
#define  IUD_INVALID (iud_t) NULL

/** \brief Creates a datablock of \p size floats 
 * \return Returns the datablock or IUD_INVALID in case of a failure.
 */
iud_t iufDataCreate
(
    int size  ///< The number of floats to allocate
);

/** \brief Deletes a datablock of \p size floats 
 * \return Returns the datablock or IUD_INVALID in case of a failure.
 */
int iufDataDelete
(
    iud_t data
);


// operations
int iufDataCompare
(
    iud_t reference,
    iud_t actual
);

int iufDataGetSize
(
    iud_t data
);

float *iufDataGetPointer
(
    iud_t data
);

int iufDataFill
(
    iud_t data,
    double *input,
    int length
);


#endif //IUFLIBRARY_IUFDATA_H

