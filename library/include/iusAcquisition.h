//
// Created by Ruijzendaal on 14/03/2018.
//
#ifndef IUSLIBRARY_IUSHLACQUISITION_H
#define IUSLIBRARY_IUSHLACQUISITION_H

#include <iusTypes.h>

// ADT
struct IusAcquisition;
typedef  struct IusAcquisition IusAcquisition;
/** Meta data to describe an acquisition. Important for Ultrasound processing is to know the \p speedOfSound in m/s. In addition is requested to report 
 *  the \p date of the experiment as an int in jjjjmmdd format together with a \p pDescription a string describing further acquisition notes.
 */
typedef  IusAcquisition    * iua_t;
#define IUA_INVALID (iua_t) NULL

/** \brief Create an acquisition object
 * \return Returns an acquisition object or #IUA_INVALID in case of an error.
 */
iua_t iusAcquisitionCreate
(
    float speedOfSound,       ///< speed of sound in m/s */
    int date,                 ///< interger concatenation of year-month-day  */
    const char *pDescription  ///< Acquisition notes */
);

/** \brief Deletes an acquisition object
 * \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE otherwise.
 */
int iusAcquisitionDelete
(
    iua_t acquisition         ///< The #IusAcquisition to delete
);

/** \brief Gets the speed of sound in m/s
 * \return Returns the speed of sound or -1 in case of an error.
 */
float iusAcquisitionGetSpeedOfSound
(
    iua_t acquisition         ///< The #IusAcquisition of interest
);

/** \brief Gets the date of the acquisition
 * \return Returns the date or -1 in case of an error.
 */
int iusAcquisitionGetDate
(
    iua_t acquisition         ///< The #IusAcquisition of interest
);

/** \brief Gets the description text of he acquisition
 * \return Returns the notes or NULL in case of an error.
 */
char *iusAcquisitionGetDescription
(
    iua_t acquisition          ///< The #IusAcquisition of interest
);

/** \brief Compares two acquisitions with each other
 * \return Returns #IUS_TRUE when the objects are equal and #IUS_FALSE otherwise.
 */
IUS_BOOL iusAcquisitionCompare
(
    iua_t reference,           ///< The #IusAcquisition to compare to
    iua_t actual               ///< The #IusAcquisition to compare with
);

#endif //IUSLIBRARY_IUSHLACQUISITION_H
