// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSRECEIVESETTINGSDICTADT_H
#define IUSLIBRARY_IUSRECEIVESETTINGSDICTADT_H

#include <hashmap.h>

// ADT
struct HashableReceiveSettings
{
    iurs_t receiveSettings;
    char key[256];
};

typedef struct HashableReceiveSettings HashableReceiveSettings;

struct IusReceiveSettingsDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
    char **keys;
};
#endif //IUSLIBRARY_IUSRECEIVESETTINGSDICTADT_H
