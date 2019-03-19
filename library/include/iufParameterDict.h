// Created by nlv09165 on 12/09/2018.
#ifndef IUFLIBRARY_IUFPARAMETERDICT_H
#define IUFLIBRARY_IUFPARAMETERDICT_H


// ADT
typedef struct IufParameterDict IufParameterDict;
/** Simple parameters {key, value} string pairs are contained in a dictionary to be used as algorithm arguments. 
 * Typically, first create an empty dictionary and then add {key,value} pairs with iufParameterDictSet()
 */
typedef IufParameterDict *iupad_t;
#define  IUPAD_INVALID (iupad_t) NULL

/** \brief Creates an empty paramter dictionary 
 * \return Returns the created dictionary or IUPAD_INVALID in case of an error
 */
iupad_t iufParameterDictCreate
(
    void
);

/** \brief Delete the paramter dictionary including all its content 
 * \return Returns #IUF_E_OK when successful and #IUF_ERR_VALUE in case the dictionary is invalid.
 */
int iufParameterDictDelete
(
    iupad_t dict ///< the #IufParameterDict to be deleted
);

/** \brief Checks if two #IufParameterDict objects are equal.
 * \return Returns #IUF_TRUE if both #IufParameterDict objects are equal and #IUF_FALSE otherwise
 */
int iufParameterDictCompare
(
    iupad_t reference, ///< The #IufParameterDict to compare to
    iupad_t actual     ///< The #IufParameterDict to compare with
);

/** \brief Gets the number of parameter entries in the dictionary
 * \return Returns the number of parameters or -1 in case of an error.
*/
size_t iufParameterDictGetSize
(
    iupad_t dict ///< The #IufParameterDict of interest
);

/** \brief Gets the \p value of parameter \p key from the dictionary
 * \return Returns \p value of parameters \p key or NULL in case it could not be found.
*/
char * iufParameterDictGet
(
    iupad_t dict,  ///< The #IufParameterDict of interest
    char * key     ///< The parameter to search for
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iufTransmitApodizationDictGetSize) of keys.
 */
char **iufParameterDictGetKeys
(
    iupad_t dict   ///< The #IufParameterDict of interest
);

/** \brief Add a {key,value} pair to the dictionary.
 * /return Returns #IUF_E_OK when successful and #IUF_ERR_VALUE in case of invalid paramters.
 */
int iufParameterDictSet
(
    iupad_t dict,  ///< The #IufParameterDict of interest
    char * key,    ///< The parameter name
    char * value   ///< The parameter value
);
#endif //IUFLIBRARY_IUFPARAMETERDICT_H
