#ifndef IUFLIBRARY_IUFHL2DNONPARAMETRICSOURCEIMP_H
#define IUFLIBRARY_IUFHL2DNONPARAMETRICSOURCEIMP_H


#include <hdf5.h>
#include <iuf2DNonParametricSource.h>

/** \brief Saves a 2D non-parametric ultrasound source to an HDF5 \p handle
 * \return Returns #IUF_E_OK on success or #IUF_ERR_VALUE in case of an error.
 */
int iuf2DNonParametricSourceSave
(
    iu2dnps_t source,               ///< The 2D non-parameric source to save
    hid_t handle					///< The HDF5 handle to save to
);

/** \brief Loads a 2D non-parametric ultrasound source fom a HDF5 \p handle
 * \return Returns the 2D non-parametric source when successfull or #IU2DP_INVALID in case of an error.
 */
iu2dnps_t iuf2DNonParametricSourceLoad
(
    hid_t handle					///< The HDF5 handle to load from
);

#endif //IUFLIBRARY_IUFHL2DNONPARAMETRICSOURCEIMP_H
