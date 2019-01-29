// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLPULSEDICT_H
#define IUSLIBRARY_IUSHLPULSEDICT_H

#include <iusHDF5.h>
#include <iusPulse.h>

// ADT
typedef struct IusPulseDict IusPulseDict;
/** A dictionary for pulses. Typically, you first create an empty dictionary with #iusPulseDictCreate() and 
 * then add one or more #IusPulse objects with iusPulseDictSet().
 */
typedef IusPulseDict *iupd_t;
#define  IUPD_INVALID (iupd_t) NULL

/** \brief Create an empty dictionary of #IusPulse objects.
 * \return Returns the created object or IUPD_INVALID in case of an error. 
 */
iupd_t iusPulseDictCreate
(
    void
);

/** \brief Delete a dictionary without deleting its #IusPulse objects.
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error. 
 */
int iusPulseDictDelete
(
    iupd_t dict ///< The pulse dictionary of interest
);

/** \brief Deletes the dictionary and all its #IusPulse objects.
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error. 
 */
int iusPulseDictDeepDelete
(
    iupd_t dict ///< The pulse dictionary of interest
);

/** \brief Check if two pulse dictionaries are equal.
 * \return Return #IUS_TRUE when the two disctionaries are equal and #IUS_FALSE otherwise.
 */
int iusPulseDictCompare
(
    iupd_t reference,      ///< The pulse dictionary to compare to
    iupd_t actual          ///< The pulse dictionary to compare with
);

/** \brief Get the number of #IusPulse objects in the dictionary.
 * \return Returns the number of #IusPulse objects or -1 in case of an error.
 */
size_t iusPulseDictGetSize
(
    iupd_t dict  ///< The pulse dictionary of interest
);

/** \brief Get the #IusPulse from the dictionary that has label \p key.
 * \return Returns the requested #IusPulse or #IUPD_INVALID if it was not found.
 */
iup_t iusPulseDictGet
(
    iupd_t list,
    char * key
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iusTransmitApodizationDictGetSize) of keys.
 */
 char **iusPulseDictGetKeys
(
    iupd_t dict   ///< The pulse dictionary of interest
);

/** \brief Add the #IusPulse to the dictionary with label \p key.
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
 */
int iusPulseDictSet
(
    iupd_t dict,   ///< The pulse dictionary of interest
    char * key,    ///< The label of the #IusPulse 
    iup_t member   ///< The #IusPulse
);

#endif //IUSLIBRARY_IUSHLPULSEDICT_H
