//
// Created by nlv12901 on 11/05/2019.
//
#ifndef IUFLIBRARY_IUFRECEIVECHANNEMAPADT_H
#define IUFLIBRARY_IUFRECEIVECHANNEMAPADT_H

struct IufReceiveChannelMap
{
    int numChannels;
	int *map;
	float   *startDelay;            /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
} ;

// SWIG binding
#ifdef SWIG
%extend IufReceiveChannelMap {
        ~IufReceiveChannelMap() {
            iufReceiveChannelMapDelete($self);
        }
}
#endif

#endif //IUFLIBRARY_IUFRECEIVECHANNELMAPADT_H
