// Created by nlv09165 on 08/08/2018.
#ifndef IUSLIBRARY_IUSHLTGC_H
#define IUSLIBRARY_IUSHLTGC_H

// ADT
typedef struct IusTGC IusTGC;
/** Time Gain Control defines a function of the receiver gain over time. The function is described by \p numTGCValues of 
 * \p [time,gain] pairs. The time values should be provided and kept in a strictly ascending order.   
 */
typedef IusTGC *iutgc_t;
#define  IUTGC_INVALID (iutgc_t) NULL

/** \brief Create an empty TGC function of \p numTGC values 
 * \return Returns the created TGC object or NULL in case of a failure
 */
iutgc_t iusTGCCreate
(
    int numTGCValues ///< The number of \p [time,gain] pairs to allocate
);

/** \brief Frees the memory of the TGC object 
 * \return Returns the created TGC object or NULL in case of a failure
 */
int iusTGCDelete
(
    iutgc_t tgc ///< The object to delete
);

/** \brief Checks if two TGCs are equal
 *  \return Returns #IUS_TRUE if the TGCs are equal and #IUS_FALSE otherwise
 */
int iusTGCCompare
(
    iutgc_t reference, ///< The TGC object to compare to
    iutgc_t actual     ///< The TGC object to compare with
);

/** \brief Get the number of \p [time,gain] pairs that the TGC function has
 *  \return Returns the number of \p [time,gain] pairs
 */
int iusTGCGetNumValues
(
    iutgc_t tgc ///< The TGC object of interest
);

/** \brief Get the \p time value of a point in the TGC function at \p index
 *  \return The time value
 */
float iusTGCGetTime
(
    iutgc_t tgc, ///< THe TGC object of interest
    int index       ///< The index of the TGC function
);

/** \brief Get the \p gain value of a point in the TGC function at \p index
 *  \return The gain value
 */
float iusTGCGetGain
(
    iutgc_t tgc, ///< The TGC object of interest
    int index       ///< The index of the TGC function
);

/** \brief Set a \p [time,gain] pair of the TGC function at \p index
 *  \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error
 */
int iusTGCSet
(
    iutgc_t tgc, ///< The TGC function of interest
    int index,      ///< The index of the TGC function to replace the time, gain pair 
    float time,     ///< The time value to set
    float gain      ///< The gain value to set
);

#endif //IUSLIBRARY_IUSHLTGC_H
