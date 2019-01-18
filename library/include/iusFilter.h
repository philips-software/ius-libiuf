// Created by nlv09165 on 08/08/2018.
#ifndef IUSLIBRARY_IUSFIRFILTER_H
#define IUSLIBRARY_IUSFIRFILTER_H

#define IUS_PATH_FIRFILTER_KERNELSIZE "KernelSize"
#define IUS_PATH_FIRFILTER_COEFFICIENTS "Coefficients"

// ADT
typedef struct IusFirFilter IusFirFilter;
/** A FIRFilter is described by \p kernelSize of filter coefficients. */
typedef IusFirFilter *iuff_t;
#define  IUFIRFILTER_INVALID (iuff_t) NULL

/** \brief Create a FIR filter with n,m coefficients of \p kernelSize values
* \return Returns the created coefficient array or NULL in case of a failure
*/
iuff_t iusFirFilterCreate
(
	int kernelSize ///< The number of filter coefficients
);

/** \brief Frees the memory of the FirFilter object
* \return Returns the created FirFilter object or NULL in case of a failure
*/
int iusFirFilterDelete
(
	iuff_t iusFirFilter ///< The object to delete
);

/** \brief Checks if two FirFilters are equal
*  \return Returns #IUS_TRUE if the filters are equal and #IUS_FALSE otherwise
*/
int iusFirFilterCompare
(
	iuff_t reference, ///< The FirFilter object to compare to
	iuff_t actual     ///< The FirFilter object to compare with
);

/** \brief Get the number of filter coefficients that the FirFilter has
*  \return Returns the number of filter coefficients
*/
int iusFirFilterGetKernelSize
(
	iuff_t iusFirFilter ///< The FirFilter object of interest   
);

/** \brief Get the \p gain value of a point in the TGC function at \p index
*  \return The gain value
*/
float iusFirFilterGetCoefficient
(
	iuff_t iusFirFilter, ///< The FirFilter object of interest
	int index            ///< The index of the kernel
);

/** \brief Set a Filter COefficient at \p index
*  \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error
*/
int iusFirFilterSetCoefficient
(
	iuff_t iusFirFilter,   ///< The FirFilter of interest
	int index,             ///< The index of the kernel
	float coefficient     ///< The filter coefficient value
);

#endif //IUSLIBRARY_IUSFIRFILTER_H
