#ifndef IUSLIBRARY_IUSTRANSMITAPODIZATIONDICTADT_H
#define IUSLIBRARY_IUSTRANSMITAPODIZATIONDICTADT_H

// ADT
#include <hashmap.h>
#include <iusTransmitApodization.h>

struct HashableTransmitApodization
{
	iuta_t transmitApodization;
	char key[256];
};

typedef struct HashableTransmitApodization HashableTransmitApodization;

struct IusTransmitApodizationDict
{
	struct hashmap map;
	IUS_BOOL deepDelete;
    char **keys;
};

#endif