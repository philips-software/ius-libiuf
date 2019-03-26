// Created by nlv09165 on 03/05/2018.
#ifndef IUFLIBRARY_IUFHLPULSEDICT_H
#define IUFLIBRARY_IUFHLPULSEDICT_H

#include <iufHDF5.h>
#include <iufPulse.h>

// ADT
typedef struct IufPulseDict IufPulseDict;
/** A dictionary for pulses. Typically, you first create an empty dictionary with #iufPulseDictCreate() and
 * then add one or more #IufPulse objects with iufPulseDictSet().
 */
typedef IufPulseDict *iupd_t;
#define  IUPD_INVALID (iupd_t) NULL

/** \brief Create an empty dictionary of #IufPulse objects.
 * \return Returns the created object or IUPD_INVALID in case of an error. 
 */
iupd_t iufPulseDictCreate
(
    void
);

/** \brief Delete a dictionary without deleting its #IufPulse objects.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an error.
 */
int iufPulseDictDelete
(
    iupd_t dict ///< The pulse dictionary of interest
);

/** \brief Deletes the dictionary and all its #IufPulse objects.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an error.
 */
int iufPulseDictDeepDelete
(
    iupd_t dict ///< The pulse dictionary of interest
);

/** \brief Check if two pulse dictionaries are equal.
 * \return Return #IUF_TRUE when the two disctionaries are equal and #IUF_FALSE otherwise.
 */
int iufPulseDictCompare
(
    iupd_t reference,      ///< The pulse dictionary to compare to
    iupd_t actual          ///< The pulse dictionary to compare with
);

/** \brief Get the number of #IufPulse objects in the dictionary.
 * \return Returns the number of #IufPulse objects or -1 in case of an error.
 */
size_t iufPulseDictGetSize
(
    iupd_t dict  ///< The pulse dictionary of interest
);

/** \brief Get the #IufPulse from the dictionary that has label \p key.
 * \return Returns the requested #IufPulse or #IUPD_INVALID if it was not found.
 */
iup_t iufPulseDictGet
(
    iupd_t list,
    char * key
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iufTransmitApodizationDictGetSize) of keys.
 */
 char **iufPulseDictGetKeys
(
    iupd_t dict   ///< The pulse dictionary of interest
);

/** \brief Add the #IufPulse to the dictionary with label \p key.
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
 */
int iufPulseDictSet
(
    iupd_t dict,   ///< The pulse dictionary of interest
    char * key,    ///< The label of the #IufPulse
    iup_t member   ///< The #IufPulse
);

/** \brief Remove the #IufPulse from the dictionary with label \p key.
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
 */
int iufPulseDictRemove
(
    iupd_t dict,   ///< The pulse dictionary of interest
    char * key     ///< The label of the #IufPulse
);
#endif //IUFLIBRARY_IUFHLPULSEDICT_H
