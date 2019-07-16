#ifndef IUFLIBRARY_IUFHLRECEIVESETTINGSDICT_H
#define IUFLIBRARY_IUFHLRECEIVESETTINGSDICT_H

#include <iufHDF5.h>
#include <iufReceiveSettings.h>

// ADT
typedef struct IufReceiveSettingsDict IufReceiveSettingsDict;
/** A dictionary of #IufReceiveSettings. Typically, first an empty dictionary is created with #iufReceiveSettingsDictCreate()
 * and then #IufReceiveSettings are added with #iufReceiveSettingsDictSet() or retreived with #iufReceiveSettingsDictGet()
 */
typedef IufReceiveSettingsDict *iursd_t;
#define  IURSD_INVALID (iursd_t) NULL

/** \brief create an empty dictionary
 * \return Returns the created dictionary or NULL in case of an error
 */ 
iursd_t iufReceiveSettingsDictCreate
(
    void
);

/** \brief Delete the dictionary without deleting its #IufReceiveSettings
 * \return #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an invalid argument.
*/
int iufReceiveSettingsDictDelete
(
    iursd_t dict ///< The dictionary of interest
);

/** \brief Delete the dictionary and its #IufReceiveSettings
 * \return #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
*/
int iufReceiveSettingsDictDeepDelete
(
    iursd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other
 * \return #IUF_TRUE in case the dictionaries are equal and #IUF_FALSE otherwise.
*/
int iufReceiveSettingsDictCompare
(
    iursd_t reference, ///< The dictionary to compare to
    iursd_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of entries in the dictionary
 * \return Returns the number of #IufReceiveSettings of the dictionary, or -1 in case of an error.
*/
size_t iufReceiveSettingsDictGetSize
(
    iursd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IufReceiveSettings from the dictionary that has the label \p key
 * \return Returns the requested #IufReceiveSettings or IURS_INVALID in case of an error
*/
iurs_t iufReceiveSettingsDictGet
(
    iursd_t dict,      ///< The dictionary of interest
    char * key         ///< The label of the receiveSettings to return
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iufTransmitApodizationDictGetSize) of keys.
 */
char **iufReceiveSettingsDictGetKeys
(
    iursd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IufReceiveSettings from the dictionary that has the label \p key
 * \return Returns the requested #IufReceiveSettings or IURS_INVALID in case of an error
*/
int iufReceiveSettingsDictSet
(
    iursd_t dict,   ///< The dictionary of interest
    char * key,     ///< The label of the #IufReceiveSettings that to be added
    iurs_t member   ///< The #IufReceiveSettings object
);

#endif //IUFLIBRARY_IUFHLRECEIVESETTINGSDICT_H
