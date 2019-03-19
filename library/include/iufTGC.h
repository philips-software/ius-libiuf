// Created by nlv09165 on 08/08/2018.
#ifndef IUFLIBRARY_IUFHLTGC_H
#define IUFLIBRARY_IUFHLTGC_H

// ADT
typedef struct IufTGC IufTGC;
/** Time Gain Control defines a function of the receiver gain over time. The function is described by \p numTGCValues of 
 * \p [time,gain] pairs. The time values should be provided and kept in a strictly ascending order.   
 */
typedef IufTGC *iutgc_t;
#define  IUTGC_INVALID (iutgc_t) NULL

/** \brief Create an empty TGC function of \p numTGC values 
 * \return Returns the created TGC object or NULL in case of a failure
 */
iutgc_t iufTGCCreate
(
    int numTGCValues ///< The number of \p [time,gain] pairs to allocate
);

/** \brief Frees the memory of the TGC object 
 * \return Returns the created TGC object or NULL in case of a failure
 */
int iufTGCDelete
(
    iutgc_t tgc ///< The object to delete
);

/** \brief Checks if two TGCs are equal
 *  \return Returns #IUF_TRUE if the TGCs are equal and #IUF_FALSE otherwise
 */
int iufTGCCompare
(
    iutgc_t reference, ///< The TGC object to compare to
    iutgc_t actual     ///< The TGC object to compare with
);

/** \brief Get the number of \p [time,gain] pairs that the TGC function has
 *  \return Returns the number of \p [time,gain] pairs
 */
int iufTGCGetNumValues
(
    iutgc_t tgc ///< The TGC object of interest
);

/** \brief Get the \p time value of a point in the TGC function at \p index
 *  \return The time value
 */
float iufTGCGetTime
(
    iutgc_t tgc, ///< THe TGC object of interest
    int index       ///< The index of the TGC function
);

/** \brief Get the \p gain value of a point in the TGC function at \p index
 *  \return The gain value
 */
float iufTGCGetGain
(
    iutgc_t tgc, ///< The TGC object of interest
    int index       ///< The index of the TGC function
);

/** \brief Set a \p [time,gain] pair of the TGC function at \p index
 *  \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an error
 */
int iufTGCSet
(
    iutgc_t tgc, ///< The TGC function of interest
    int index,      ///< The index of the TGC function to replace the time, gain pair 
    float time,     ///< The time value to set
    float gain      ///< The gain value to set
);

#endif //IUFLIBRARY_IUFHLTGC_H
