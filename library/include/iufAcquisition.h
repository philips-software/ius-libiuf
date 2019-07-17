#ifndef IUFLIBRARY_IUFHLACQUISITION_H
#define IUFLIBRARY_IUFHLACQUISITION_H

#include <iufTypes.h>

// ADT
struct IufAcquisition;
typedef  struct IufAcquisition IufAcquisition;
/** Meta data to describe an acquisition. Important for Ultrasound processing is to know the \p speedOfSound in m/s. In addition is requested to report 
 *  the \p date of the experiment as an int in jjjjmmdd format together with a \p pDescription a string describing further acquisition notes.
 */
typedef  IufAcquisition    * iua_t;
#define IUA_INVALID (iua_t) NULL

/** \brief Create an acquisition object
 * \return Returns an acquisition object or #IUA_INVALID in case of an error.
 */
iua_t iufAcquisitionCreate
(
    float speedOfSound,       ///< speed of sound in m/s */
    int date,                 ///< interger concatenation of year-month-day  */
    const char *pDescription  ///< Acquisition notes */
);

/** \brief Deletes an acquisition object
 * \return Returns #IUF_E_OK when successful and #IUF_ERR_VALUE otherwise.
 */
int iufAcquisitionDelete
(
    iua_t acquisition         ///< The #IufAcquisition to delete
);

/** \brief Gets the speed of sound in m/s
 * \return Returns the speed of sound or -1 in case of an error.
 */
float iufAcquisitionGetSpeedOfSound
(
    iua_t acquisition         ///< The #IufAcquisition of interest
);

/** \brief Gets the date of the acquisition
 * \return Returns the date or -1 in case of an error.
 */
int iufAcquisitionGetDate
(
    iua_t acquisition         ///< The #IufAcquisition of interest
);

/** \brief Gets the description text of he acquisition
 * \return Returns the notes or NULL in case of an error.
 */
char *iufAcquisitionGetDescription
(
    iua_t acquisition          ///< The #IufAcquisition of interest
);

/** \brief Compares two acquisitions with each other
 * \return Returns #IUF_TRUE when the objects are equal and #IUF_FALSE otherwise.
 */
IUF_BOOL iufAcquisitionCompare
(
    iua_t reference,           ///< The #IufAcquisition to compare to
    iua_t actual               ///< The #IufAcquisition to compare with
);

#endif //IUFLIBRARY_IUFHLACQUISITION_H
