// Created by nlv09165 on 15/01/2019.
#ifndef IUFLIBRARY_IUFDATASTREAMDICT_H
#define IUFLIBRARY_IUFDATASTREAMDICT_H

#include <iufHDF5.h>
#include <iufDataStream.h>

// ADT
typedef struct IufDataStreamDict IufDataStreamDict;
/** A dictionary of #IufDataStream. Typically, first an empty dictionary is created with #iufDataStreamDictCreate()
 * and then #IufDataStream are added with #iufDataStreamDictSet() or retreived with #iufDataStreamDictGet()
 */
typedef IufDataStreamDict *iudsd_t;
#define  IUDSD_INVALID (iudsd_t) NULL

/** \brief create an empty dictionary
 * \return Returns the created dictionary or NULL in case of an error
 */
iudsd_t iufDataStreamDictCreate
(
  void
);

/** \brief Delete the dictionary without deleting its #IufDataStream
 * \return #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufDataStreamDictDelete
(
  iudsd_t dict ///< The dictionary of interest
);

/** \brief Delete the dictionary and its #IufDataStream
 * \return #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
 */
int iufDataStreamDictDeepDelete
(
  iudsd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other
 * \return #IUF_TRUE in case the dictionaries are equal and #IUF_FALSE otherwise.
 */
int iufDataStreamDictCompare
(
  iudsd_t reference, ///< The dictionary to compare to
  iudsd_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of entries in the dictionary
 * \return Returns the number of #IufDataStream of the dictionary, or -1 in case of an error.
 */
size_t iufDataStreamDictGetSize
(
  iudsd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IufDataStream from the dictionary that has the label \p key
 * \return Returns the requested #IufDataStream or IURS_INVALID in case of an error
 */
iuds_t iufDataStreamDictGet
(
  iudsd_t dict,      ///< The dictionary of interest
  char * key         ///< The label of the demodulation to return
);

/** \brief Get the #IufPatternList object with label \p key from the dictionary.
 * \return Returns #IufPatternList \p key or #IUPALD_INVALID if it is not found.
 */
char **iufDataStreamDictGetKeys
(
  iudsd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IufDataStream from the dictionary that has the label \p key
 * \return Returns the requested #IufDataStream or IURS_INVALID in case of an error
 */
int iufDataStreamDictSet
(
  iudsd_t dict,   ///< The dictionary of interest
  char * key,     ///< The label of the #IufDataStream that to be added
  iuds_t member   ///< The #IufDataStream object
);

/** \brief Remove a #IufDataStream from the dictionary that has the label \p key
 * \return Returns IUF_ERR_VALUE in case the key was noet found or IUF_E_OK when successful
 */
int iufDataStreamDictRemove
(
  iudsd_t dict,   ///< The dictionary of interest
  char * key       ///< The label of the #IufDataStream that is to be removed
);
#endif //IUFLIBRARY_IUFDATASTREAMDICT_H