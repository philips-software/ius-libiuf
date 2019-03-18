// Created by nlv09165 on 10/08/2018.
#ifndef IUFLIBRARY_IUFRECEIVESETTINGSDICTADT_H
#define IUFLIBRARY_IUFRECEIVESETTINGSDICTADT_H

#include <hashmap.h>

// ADT
struct HashableReceiveSettings
{
    iurs_t receiveSettings;
    char key[256];
};

typedef struct HashableReceiveSettings HashableReceiveSettings;

struct IufReceiveSettingsDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
};
#endif //IUFLIBRARY_IUFRECEIVESETTINGSDICTADT_H
