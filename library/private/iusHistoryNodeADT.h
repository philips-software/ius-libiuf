#ifndef IUSLIBRARY_IUSHISTORYNODEADT_H
#define IUSLIBRARY_IUSHISTORYNODEADT_H
// ADT
struct IusHistoryNode
{

    char                *pId;
    char                *pType;
    int                 numberOfParents;
    iuhnl_t             parents;
    int                 numberOfParameters;
    iupad_t             parameters;
    void *              instanceData;
    IUS_BOOL            deepDelete;

} ;

// SWIG binding
#ifdef SWIG

%extend IusHistoryNode {             // Attach these functions to struct Vector
        IusHistoryNode(char *nodeType) {
            return iusHistoryNodeCreate(nodeType);
        }

        ~IusHistoryNode() {
            iusHistoryNodeDelete($self);
        }

        char *Type()
        {
            return iusHistoryNodeGetType($self);
        }

        int NumParams()
        {
            return iusHistoryNodeGetNumParams($self);
        }

        int NumParents()
        {
            return iusHistoryNodeGetNumParents($self);
        }

        char *__str__() {
            static char temp[256];
            sprintf(temp, "IusHistoryNode [%s]\n", $self->pType);
            return &temp[0];
        }
};

#endif

#endif // IUSLIBRARY_IUSHISTORYNODEADT_H
