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

#endif // IUSLIBRARY_IUSHISTORYNODEADT_H
