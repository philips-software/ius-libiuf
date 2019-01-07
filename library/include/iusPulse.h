//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLPULSE_H
#define IUSLIBRARY_IUSHLPULSE_H

#include <iusTypes.h>

// ADT
struct   IusPulse;
typedef  struct IusPulse IusPulse;
/** The pulse describes a waveform of the transmit pulse as a function of time and can be described parametric or as a
 * piecewise linear approximation using a list of [time,value] pairs. See #IusParametricPulse and #IusNonParametricPulse 
 * for details. Use the specific functions #iusParametricPulseCreate() and #iusNonParametricPulseCreate() to construct a pulse.
 */
typedef  IusPulse    * iup_t;
#define  IUP_INVALID (iup_t) NULL

typedef enum {
  IUS_INVALID_PULSETYPE = 0,
  IUS_PARAMETRIC_PULSETYPE,
  IUS_NON_PARAMETRIC_PULSETYPE
} IusPulseType;

/** \brief Deletes a #IusPulse object
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error
 */
int iusPulseDelete
(
    iup_t pulse          ///< The #IusPulse of interest.
);

/** \brief Compare two #IusPulse objects with each other
 * \return Returns #IUS_TRUE when the #IusPulse objects are equal and #IUS_FALSE otherwise.
 */
IUS_BOOL  iusPulseCompare
(
    iup_t reference,     ///< The #IusPulse to compare to
    iup_t actual        ///< The #IusPulse to compare with
);

/** \brief Get the #IusPulse type , i.e. IUS_PARAMETRIC_PULSETYPE or IUS_NON_PARAMETRIC_PULSETYPE
 * \return Returns the pulse type of \p pulse.
 */
IusPulseType iusPulseGetType
(
    iup_t pulse ///< The #IusPulse of interest
);

#endif //IUSLIBRARY_IUSHLPULSE_H
