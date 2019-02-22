//
// Created by Ruijzendaal on 31/07/2018.
//

#ifndef IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCEIMP_H


#include <hdf5.h>
#include <ius2DNonParametricSource.h>

/** \brief Saves a 2D non-parametric ultrasound source to an HDF5 \p handle
 * \return Returns #IUS_E_OK on success or #IUS_ERR_VALUE in case of an error.
 */
int ius2DNonParametricSourceSave
(
    ius_t source,               ///< The 2D non-parameric source to save
    hid_t handle					///< The HDF5 handle to save to
);

/** \brief Loads a 2D non-parametric ultrasound source fom a HDF5 \p handle
 * \return Returns the 2D non-parametric source when successfull or #IU2DP_INVALID in case of an error.
 */
ius_t ius2DNonParametricSourceLoad
(
    hid_t handle					///< The HDF5 handle to load from
);

#endif //IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCEIMP_H
