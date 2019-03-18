// Created by nlv09165 on 07/09/2018.
#ifndef IUFLIBRARY_IUFHISTORYNODELIST_H
#define IUFLIBRARY_IUFHISTORYNODELIST_H

#include <iufHistoryNode.h>

/** \brief Create an empty list of #IufHistoryNode for \p numHistoryNode nodes
 * \return Returns the allocated list or #IUHNL_INVALID in case of an invalid argument 
 */
iuhnl_t iufHistoryNodeListCreate
(
    int numHistoryNodes             ///< The number of nodes in the list
);

/** \brief Delete a #IufHistoryNodeList and all its entries
 * \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
 */
int iufHistoryNodeListDelete
(
    iuhnl_t list   ///< the list to be deleted
);

/** \brief Compare two #IufHistoryNodeList objects with each other
 * \return Returns #IUF_TRUE when the lists are equal and #IUF_FALSE otherwise.
 */
int iufHistoryNodeListCompare
(
    iuhnl_t reference,     ///< The #IufHistoryNodeList to compare to
    iuhnl_t actual         ///< The #IufHistoryNodeList to compare with
);

/** \brief Get the number of nodes in the #IufHistoryNodeList
 * \return Returns the number of nodes of the list when the lists are equal and #IUF_FALSE otherwise.
 */
int iufHistoryNodeListGetSize
(
    iuhnl_t list           ///< The #IufHistoryNodeList of interest
);

/** \brief Gets the node from the list at \p index.  
 * \return Returns the #IufHistoryNode at \p index or NULL in case of invalid arguments.
 */
iuhn_t iufHistoryNodeListGet
(
    iuhnl_t list,          ///< The #IufHistoryNodeList of interest
    int index              ///< The index of the node in the list
);

/** \brief Sets the node \p member in list at \p index.  
 * \return Returns #IUF_E_OK when successful and #IUF_ERR_VALUE otherwise.
 */
int iufHistoryNodeListSet
(
    iuhnl_t list,          ///< The #IufHistoryNodeList of interest
    iuhn_t member,         ///< The #IufHistoryNode to set
    int index              ///< The index in the list
);

#endif //IUFLIBRARY_IUFHISTORYNODELIST_H
