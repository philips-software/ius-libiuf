#ifndef IUS_NODE_H
#define IUS_NODE_H

#include <hdf5.h>

#define IUSLIBRARY_IMPLEMENTATION
#define MAX_TYPE_LENGTH 40
#define MAX_ID_LENGTH   40
#define MAX_PARENTS     10

struct IusNode
{
    char                pId[MAX_ID_LENGTH];
    char                pType[MAX_TYPE_LENGTH];
    int                 numberOfParents;
    struct IusNode * pParents[MAX_PARENTS];
} ;

typedef struct IusNode IusNode;


// When reading a HDF5 handle to a data struct, use this method to
// automatically load all your parents.
void iusNodeLoadParents
(
    IusNode * pNode,
    hid_t     handle
);

// When writing a data struct into a HDF handle, use this method to
// automatically save all your parents.
void iusNodeSaveParents
(
    IusNode * pNode,
    hid_t     handle
);

#endif
