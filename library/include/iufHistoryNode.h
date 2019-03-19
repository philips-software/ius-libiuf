// Created by nlv09165 on 31/08/2018.
#ifndef IUFLIBRARY_IUFHISTORYNODE_H
#define IUFLIBRARY_IUFHISTORYNODE_H

#include <iufParameterDict.h>

// ADT
typedef struct IufHistoryNodeList IufHistoryNodeList;
/** The parents of data objects are recorded in #IufHistoryNode objects. Since there can be more than 1, we store them in a list.
 * Typically one knows the number of nodes a file depends on, so the list is created with a predefined fixed size and
 * nodes are set at a specific index. */
typedef IufHistoryNodeList *iuhnl_t;
#define  IUHNL_INVALID (iuhnl_t) NULL
/** A node itself can have a node as a parent. It has an instance type and that the processing parameters 
 * used to create the current instance */  
typedef struct IufHistoryNode IufHistoryNode;

typedef IufHistoryNode *iuhn_t;
#define  IUHN_INVALID (iuhn_t) NULL

/** \brief Creates a history node of the type described by the string \p pNodeType 
 * \return It returns the history node when successful or #IUHN_INVALID in case the node type was invalid */ 
iuhn_t iufHistoryNodeCreate
(
    char *pNodeType          ///< The node type description
);

/** \brief Delete a history node  
 * \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error */
int iufHistoryNodeDelete
(
    iuhn_t historyNode    ///< The node to be deleted
);

/** \brief Compares two history nodes with each other, including their Id (so a rerun would be different) 
 *  \return Returns #IUF_TRUE if the nodes are equal and #IUF_FALSE otherwise.
 */
int iufHistoryNodeCompareWithId
(
    iuhn_t reference,        ///< The node to compare to
    iuhn_t actual            ///< The node to compare with
);

/** \brief Compares two history nodes with each other, except their Ids (so could be a rerun)  
 *  \return Returns #IUF_TRUE if the nodes are equal and #IUF_FALSE otherwise.
 */
int iufHistoryNodeCompare
(
    iuhn_t reference,        ///< The node to compare to
    iuhn_t actual            ///< The node to compare with
);

/** \brief Gets the number of parents that a node has.  
 *  \return Returns the number of parents or  -1 in case of in error
 */
int iufHistoryNodeGetNumParents
(
    iuhn_t historyNode    ///< The node of interest
);

/** \brief Gets the number of parameters that a node has.  
 *  \return Returns the number of parameters or -1 in case of in error
 */
int iufHistoryNodeGetNumParams
(
    iuhn_t historyNode     ///< The node of interest
);

/** \brief Gets the label of the node  
 *  \return Returns the node type string or NULL in case of an invalid argument.
 */
char *iufHistoryNodeGetType
(
    iuhn_t historyNode     ///< The node of interest
);

/** \brief Gets the list of the parent nodes  
 *  \return Returns the #IufHistoryNodeList of parents or #IUHNL_INVALID in case of an error.
 */
iuhnl_t iufHistoryNodeGetParents
(
    iuhn_t historyNode     ///< The node of interest
);

/** \brief Gets the parameter dictionary  
 *  \return Returns the #IufParameterDict or #IUPAD_INVALID in case of an error.
 */
iupad_t iufHistoryNodeGetParameters
(
    iuhn_t historyNode               ///< The node of interest
);

/** \brief Sets the #IufHistoryNodeList of a parent nodes
 *  \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE otherwise.
 */
int iufHistoryNodeSetParents
(
    iuhn_t historyNode,    ///< The node of interest
    iuhnl_t parents           ///< the #IufHistoryNodeList to set
);

/** \brief Sets the #IufParameterDict of a node
 *  \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE otherwise.
 */
int iufHistoryNodeSetParameters
(
    iuhn_t historyNode,              ///< The node of interest
    iupad_t parameterDict     ///< The #IufParameterDict to set
);

#endif //IUFLIBRARY_IUFHISTORYNODE_H
