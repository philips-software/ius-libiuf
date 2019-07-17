#ifndef IUFLIBRARY_IUFHLPULSE_H
#define IUFLIBRARY_IUFHLPULSE_H

#include <iufTypes.h>

// ADT
struct   IufPulse;
typedef  struct IufPulse IufPulse;
/** The pulse describes a waveform of the transmit pulse as a function of time and can be described parametric or as a
 * piecewise linear approximation using a list of [time,value] pairs. See #IufParametricPulse and #IufNonParametricPulse
 * for details. Use the specific functions #iufParametricPulseCreate() and #iufNonParametricPulseCreate() to construct a pulse.
 */
typedef  IufPulse    * iup_t;
#define  IUP_INVALID (iup_t) NULL

typedef enum {
  IUF_INVALID_PULSETYPE = 0,
  IUF_PARAMETRIC_PULSETYPE,
  IUF_NON_PARAMETRIC_PULSETYPE
} IufPulseType;

/** \brief Deletes a #IufPulse object
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error
 */
int iufPulseDelete
(
    iup_t pulse          ///< The #IufPulse of interest.
);

/** \brief Compare two #IufPulse objects with each other
 * \return Returns #IUF_TRUE when the #IufPulse objects are equal and #IUF_FALSE otherwise.
 */
IUF_BOOL  iufPulseCompare
(
    iup_t reference,     ///< The #IufPulse to compare to
    iup_t actual        ///< The #IufPulse to compare with
);

/** \brief Get the #IufPulse type , i.e. IUF_PARAMETRIC_PULSETYPE or IUF_NON_PARAMETRIC_PULSETYPE
 * \return Returns the pulse type of \p pulse.
 */
IufPulseType iufPulseGetType
(
    iup_t pulse ///< The #IufPulse of interest
);

#endif //IUFLIBRARY_IUFHLPULSE_H
