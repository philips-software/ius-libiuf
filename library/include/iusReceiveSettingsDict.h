// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H

#include <iusHDF5.h>
#include <iusReceiveSettings.h>

// ADT
typedef struct IusReceiveSettingsDict IusReceiveSettingsDict;
/** A dictionary of #IusReceiveSettings. Typically, first an empty dictionary is created with #iusReceiveSettingsDictCreate() 
 * and then #IusReceiveSettings are added with #iusReceiveSettingsDictSet() or retreived with #iusReceiveSettingsDictGet() 
 */
typedef IusReceiveSettingsDict *iursd_t;
#define  IURSD_INVALID (iursd_t) NULL

/** \brief create an empty dictionary
 * \return Returns the created dictionary or NULL in case of an error
 */ 
iursd_t iusReceiveSettingsDictCreate
(
    void
);

/** \brief Delete the dictionary without deleting its #IusReceiveSettings
 * \return #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an invalid argument.
*/
int iusReceiveSettingsDictDelete
(
    iursd_t dict ///< The dictionary of interest
);

/** \brief Delete the dictionary and its #IusReceiveSettings
 * \return #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
*/
int iusReceiveSettingsDictDeepDelete
(
    iursd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other
 * \return #IUS_TRUE in case the dictionaries are equal and #IUS_FALSE otherwise.
*/
int iusReceiveSettingsDictCompare
(
    iursd_t reference, ///< The dictionary to compare to
    iursd_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of entries in the dictionary
 * \return Returns the number of #IusReceiveSettings of the dictionary, or -1 in case of an error.
*/
int iusReceiveSettingsDictGetSize
(
    iursd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IusReceiveSettings from the dictionary that has the label \p key
 * \return Returns the requested #IusReceiveSettings or IURS_INVALID in case of an error
*/
iurs_t iusReceiveSettingsDictGet
(
    iursd_t dict,      ///< The dictionary of interest
    char * key         ///< The label of the receiveSettings to return
);

/** \brief Get the #IusReceiveSettings from the dictionary that has the label \p key
 * \return Returns the requested #IusReceiveSettings or IURS_INVALID in case of an error
*/
int iusReceiveSettingsDictSet
(
    iursd_t dict,   ///< The dictionary of interest
    char * key,     ///< The label of the #IusReceiveSettings that to be added
    iurs_t member   ///< The #IusReceiveSettings object
);

#endif //IUSLIBRARY_IUSHLRECEIVESETTINGSDICT_H
