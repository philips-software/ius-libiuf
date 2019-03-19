//
// Created by nlv12901 on 11/05/2019.
//
#ifndef IUFLIBRARY_IUFTRANSMITAPODIZATIONADT_H
#define IUFLIBRARY_IUFTRANSMITAPODIZATIONADT_H

struct IufTransmitApodization
{
	int numElements;
	float *apodization;
};

// SWIG binding
#ifdef SWIG
%extend IufTransmitApodization {
        ~IufTransmitApodization() {
            iufTransmitApodizationDelete($self);
        }
}
#endif

#endif //IUFLIBRARY_IUFTRANSMITAPODIZATIONADT_H
