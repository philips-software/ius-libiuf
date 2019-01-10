// Created by nlv09165 on 12/09/2018.
#ifndef IUSLIBRARY_IUSPARAMETERDICT_H
#define IUSLIBRARY_IUSPARAMETERDICT_H


// ADT
typedef struct IusParameterDict IusParameterDict;
/** Simple parameters {key, value} string pairs are contained in a dictionary to be used as algorithm arguments. 
 * Typically, first create an empty dictionary and then add {key,value} pairs with iusParameterDictSet() 
 */
typedef IusParameterDict *iupad_t;
#define  IUPAD_INVALID (iupad_t) NULL

/** \brief Creates an empty paramter dictionary 
 * \return Returns the created dictionary or IUPAD_INVALID in case of an error
 */
iupad_t iusParameterDictCreate
(
    void
);

/** \brief Delete the paramter dictionary including all its content 
 * \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE in case the dictionary is invalid.
 */
int iusParameterDictDelete
(
    iupad_t dict ///< the #IusParameterDict to be deleted
);

/** \brief Checks if two #IusParameterDict objects are equal.
 * \return Returns #IUS_TRUE if both #IusParameterDict objects are equal and #IUS_FALSE otherwise
 */
int iusParameterDictCompare
(
    iupad_t reference, ///< The #IusParameterDict to compare to 
    iupad_t actual     ///< The #IusParameterDict to compare with
);

/** \brief Gets the number of parameter entries in the dictionary
 * \return Returns the number of parameters or -1 in case of an error.
*/
int iusParameterDictGetSize
(
    iupad_t dict ///< The #IusParameterDict of interest
);

/** \brief Gets the \p value of parameter \p key from the dictionary
 * \return Returns \p value of parameters \p key or NULL in case it could not be found.
*/
char * iusParameterDictGet
(
    iupad_t dict,  ///< The #IusParameterDict of interest
    char * key     ///< The parameter to search for
);

/** \brief Add a {key,value} pair to the dictionary.
 * /return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE in case of invalid paramters. 
 */
int iusParameterDictSet
(
    iupad_t dict,  ///< The #IusParameterDict of interest
    char * key,    ///< The parameter name
    char * value   ///< The parameter value
);
#endif //IUSLIBRARY_IUSPARAMETERDICT_H
