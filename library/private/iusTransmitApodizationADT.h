//
// Created by nlv12901 on 11/05/2019.
//
#ifndef IUSLIBRARY_IUSTRANSMITAPODIZATIONADT_H
#define IUSLIBRARY_IUSTRANSMITAPODIZATIONADT_H

struct IusTransmitApodization
{
	int numElements;
	float *apodization;
};

// SWIG binding
#ifdef SWIG
%extend IusTransmitApodization {
        ~IusTransmitApodization() {
            iusTransmitApodizationDelete($self);
        }
}
#endif

#endif //IUSLIBRARY_IUSTRANSMITAPODIZATIONADT_H
