#ifndef IUFLIBRARY_IUFTRANSMITAPODIZATIONDICTADT_H
#define IUFLIBRARY_IUFTRANSMITAPODIZATIONDICTADT_H

// ADT
#include <hashmap.h>
#include <iufTransmitApodization.h>

struct HashableTransmitApodization
{
	iuta_t transmitApodization;
	char key[256];
};

typedef struct HashableTransmitApodization HashableTransmitApodization;

struct IufTransmitApodizationDict
{
	struct hashmap map;
	IUF_BOOL deepDelete;
    char **kys;
};

#endif
