// Created by nlv09165 on 12/09/2018.
#ifndef IUSLIBRARY_IUSPARAMETERDICTADT_H
#define IUSLIBRARY_IUSPARAMETERDICTADT_H

#include <hashmap.h>
// ADT
struct HashableParameter
{
    char *value;
    char *key;
} ;

typedef struct HashableParameter HashableParameter;

struct IusParameterDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
    char **keys;
} ;


#endif //IUSLIBRARY_IUSPARAMETERDICTADT_H
