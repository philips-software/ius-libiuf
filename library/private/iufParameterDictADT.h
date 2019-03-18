// Created by nlv09165 on 12/09/2018.
#ifndef IUFLIBRARY_IUFPARAMETERDICTADT_H
#define IUFLIBRARY_IUFPARAMETERDICTADT_H

#include <hashmap.h>
// ADT
struct HashableParameter
{
    char *value;
    char *key;
} ;

typedef struct HashableParameter HashableParameter;

struct IufParameterDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
} ;

#endif //IUFLIBRARY_IUFPARAMETERDICTADT_H
