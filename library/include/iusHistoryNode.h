// Created by nlv09165 on 31/08/2018.
#ifndef IUSLIBRARY_IUSHISTORYNODE_H
#define IUSLIBRARY_IUSHISTORYNODE_H

#include <iusParameterDict.h>

// ADT
typedef struct IusHistoryNodeList IusHistoryNodeList;
/** The parents of data objects are recorded in #IusHistoryNode objects. Since there can be more than 1, we store them in a list.  
 * Typically one knows the number of nodes a file depends on, so the list is created with a predefined fixed size and
 * nodes are set at a specific index. */
typedef IusHistoryNodeList *iuhnl_t;
#define  IUHNL_INVALID (iuhnl_t) NULL
/** A node itself can have a node as a parent. It has an instance type and that the processing parameters 
 * used to create the current instance */  
typedef struct IusHistoryNode IusHistoryNode;

typedef IusHistoryNode *iuhn_t;
#define  IUHN_INVALID (iuhn_t) NULL

/** \brief Creates a history node of the type described by the string \p pNodeType 
 * \return It returns the history node when successful or #IUHN_INVALID in case the node type was invalid */ 
iuhn_t iusHistoryNodeCreate
(
    char *nodeType          ///< The node type description
);

/** \brief Delete a history node  
 * \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an error */ 
int iusHistoryNodeDelete
(
    iuhn_t historyNode    ///< The node to be deleted
);

/** \brief Compares two history nodes with each other, including their Id (so a rerun would be different) 
 *  \return Returns #IUS_TRUE if the nodes are equal and #IUS_FALSE otherwise. 
 */
int iusHistoryNodeCompareWithId
(
    iuhn_t reference,        ///< The node to compare to
    iuhn_t actual            ///< The node to compare with
);

/** \brief Compares two history nodes with each other, except their Ids (so could be a rerun)  
 *  \return Returns #IUS_TRUE if the nodes are equal and #IUS_FALSE otherwise. 
 */
int iusHistoryNodeCompare
(
    iuhn_t reference,        ///< The node to compare to
    iuhn_t actual            ///< The node to compare with
);

/** \brief Gets the number of parents that a node has.  
 *  \return Returns the number of parents or  -1 in case of in error
 */
int iusHistoryNodeGetNumParents
(
    iuhn_t historyNode    ///< The node of interest
);

/** \brief Gets the number of parameters that a node has.  
 *  \return Returns the number of parameters or -1 in case of in error
 */
int iusHistoryNodeGetNumParams
(
    iuhn_t historyNode     ///< The node of interest
);

/** \brief Gets the label of the node  
 *  \return Returns the node type string or NULL in case of an invalid argument.
 */
char *iusHistoryNodeGetType
(
    iuhn_t historyNode     ///< The node of interest
);

/** \brief Gets the list of the parent nodes  
 *  \return Returns the #IusHistoryNodeList of parents or #IUHNL_INVALID in case of an error.
 */
iuhnl_t iusHistoryNodeGetParents
(
    iuhn_t historyNode     ///< The node of interest
);

/** \brief Gets the parameter dictionary  
 *  \return Returns the #IusParameterDict or #IUPAD_INVALID in case of an error.
 */
iupad_t iusHistoryNodeGetParameters
(
    iuhn_t historyNode               ///< The node of interest
);

/** \brief Sets the #IusHistoryNodeList of a parent nodes  
 *  \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE otherwise.
 */
int iusHistoryNodeSetParents
(
    iuhn_t historyNode,    ///< The node of interest
    iuhnl_t parents           ///< the #IusHistoryNodeList to set
);

/** \brief Sets the #IusParameterDict of a node  
 *  \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE otherwise.
 */
int iusHistoryNodeSetParameters
(
    iuhn_t historyNode,              ///< The node of interest
    iupad_t parameterDict     ///< The #IusParameterDict to set
);

#endif //IUSLIBRARY_IUSHISTORYNODE_H
