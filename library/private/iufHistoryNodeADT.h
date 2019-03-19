#ifndef IUFLIBRARY_IUFHISTORYNODEADT_H
#define IUFLIBRARY_IUFHISTORYNODEADT_H
// ADT
struct IufHistoryNode
{

    char                *pId;
    char                *pType;
    int                 numberOfParents;
    iuhnl_t             parents;
    int                 numberOfParameters;
    iupad_t             parameters;
    void *              instanceData;
    IUF_BOOL            deepDelete;

} ;

// SWIG binding
#ifdef SWIG

%extend IufHistoryNode {             // Attach these functions to struct Vector
        IufHistoryNode(char *nodeType) {
            return iufHistoryNodeCreate(nodeType);
        }

        ~IufHistoryNode() {
            iufHistoryNodeDelete($self);
        }

        char *Type()
        {
            return iufHistoryNodeGetType($self);
        }

        int NumParams()
        {
            return iufHistoryNodeGetNumParams($self);
        }

        int NumParents()
        {
            return iufHistoryNodeGetNumParents($self);
        }

        char *__str__() {
            static char temp[256];
            sprintf(temp, "IufHistoryNode [%s]\n", $self->pType);
            return &temp[0];
        }
};

#endif

#endif // IUFLIBRARY_IUFHISTORYNODEADT_H
