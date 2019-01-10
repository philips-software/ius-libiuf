// Created by nlv09165 on 07/09/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODELIST_H
#define IUSLIBRARY_IUSHISTORYNODELIST_H

#include <include/iusHistoryNode.h>

/** \brief Create an empty list of #IusHistoryNode for \p numHistoryNode nodes 
 * \return Returns the allocated list or #IUHNL_INVALID in case of an invalid argument 
 */
iuhnl_t iusHistoryNodeListCreate
(
    int numHistoryNodes             ///< The number of nodes in the list
);

/** \brief Delete a #IusHistoryNodeList and all its entries 
 * \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an error. 
 */
int iusHistoryNodeListDelete
(
    iuhnl_t list   ///< the list to be deleted
);

/** \brief Compare two #IusHistoryNodeList objects with each other 
 * \return Returns #IUS_TRUE when the lists are equal and #IUS_FALSE otherwise. 
 */
int iusHistoryNodeListCompare
(
    iuhnl_t reference,     ///< The #IusHistoryNodeList to compare to
    iuhnl_t actual         ///< The #IusHistoryNodeList to compare with
);

/** \brief Get the number of nodes in the #IusHistoryNodeList  
 * \return Returns the number of nodes of the list when the lists are equal and #IUS_FALSE otherwise. 
 */
int iusHistoryNodeListGetSize
(
    iuhnl_t list           ///< The #IusHistoryNodeList of interest
);

/** \brief Gets the node from the list at \p index.  
 * \return Returns the #IusHistoryNode at \p index or NULL in case of invalid arguments. 
 */
iuhn_t iusHistoryNodeListGet
(
    iuhnl_t list,          ///< The #IusHistoryNodeList of interest
    int index              ///< The index of the node in the list
);

/** \brief Sets the node \p member in list at \p index.  
 * \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE otherwise. 
 */
int iusHistoryNodeListSet
(
    iuhnl_t list,          ///< The #IusHistoryNodeList of interest
    iuhn_t member,         ///< The #IusHistoryNode to set
    int index              ///< The index in the list
);

#endif //IUSLIBRARY_IUSHISTORYNODELIST_H
