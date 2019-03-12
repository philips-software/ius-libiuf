//
// Created by nlv12901 on 11/05/2019.
//
#ifndef IUSLIBRARY_IUSRECEIVECHANNEMAPADT_H
#define IUSLIBRARY_IUSRECEIVECHANNEMAPADT_H

struct IusReceiveChannelMap
{
    int numChannels;
	int *map;
	float   *startDelay;            /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
} ;

// SWIG binding
#ifdef SWIG
%extend IusReceiveChannelMap {
        ~IusReceiveChannelMap() {
            iusReceiveChannelMapDelete($self);
        }
}
#endif

#endif //IUSLIBRARY_IUSRECEIVECHANNELMAPADT_H
