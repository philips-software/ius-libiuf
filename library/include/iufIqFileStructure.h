//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iqFileStructure.h
//                  various iuf types.
//  iufVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 * \brief The iq file structure serialization paths
 */

#ifndef _IUFIQFILESTRUCTURE_H
#define _IUFIQFILESTRUCTURE_H

#define IUF_PATH_ANGLE "Angle"
#define IUF_PATH_ANGLE_PHI "Phi"
#define IUF_PATH_ANGLE_THETA "Theta"
#define IUF_PATH_POSITION "Position"
#define IUF_PATH_POSITION_X "X"
#define IUF_PATH_POSITION_Y "Y"
#define IUF_PATH_POSITION_Z "Z"
#define IUF_PATH_SIZE "Size"
#define IUF_PATH_SIZE_X "SX"
#define IUF_PATH_SIZE_Y "SY"
#define IUF_PATH_SIZE_Z "SZ"

#define IUF_PATH_ACQUISITION "Acquisition"
#define IUF_PATH_ACQUISITION_DATE "Date"
#define IUF_PATH_ACQUISITION_DESCRIPTION "Description"
#define IUF_PATH_ACQUISITION_SPEEDOFSOUND "SpeedOfSound"
#define IUF_PATH_FRAMELIST "Frames"
#define IUF_PATH_FRAMELIST_FRAME "Frame[%d]"
#define IUF_PATH_FRAME_DATAINDEX "DataIndex"
#define IUF_PATH_FRAME_PATTERNLISTLABEL "PatternListLabel"
#define IUF_PATH_FRAME_TIME "Time"
#define IUF_PATH_FRAMELIST_SIZE "Size"
#define IUF_PATH_IUFVERSION "IufVersion"
#define IUF_PATH_NUMFRAMES "NumFrames"
//#define IUF_IQFILE_PATH_PATTERNLIST "Patterns"
#define IUF_PATH_PATTERNLISTDICT "Patterns"
#define IUF_PATH_PATTERNLIST_PATTERN "Pattern[%d]"
#define IUF_PATH_PATTERNLIST_SIZE "Size"
#define IUF_PATH_PATTERN_APODIZATIONLABEL "ApodizationLabel"
#define IUF_PATH_PATTERN_CHANNELMAPLABEL "ChannelMapLabel"
//#define IUF_IQFILE_PATH_PATTERNLIST_PATTERN_PATTERNLABEL "PatternLabel"
#define IUF_PATH_PATTERN_PULSELABEL "PulseLabel"
#define IUF_PATH_PATTERN_RECEIVESETTINGSLABEL "ReceiveSettingsLabel"
#define IUF_PATH_PATTERN_SOURCELABEL "SourceLabel"
#define IUF_PATH_PATTERN_DEMODULATIONLABEL "Demodulations"
#define IUF_PATH_PATTERN_TIMEINFRAME "TimeInFrame"
#define IUF_PATH_PULSEDICT "Pulses"
#define IUF_PATH_PULSE_NUMPULSEVALUES "NumPulseValues"
#define IUF_PATH_PULSE_PULSETYPE "PulseType"
#define IUF_PATH_PULSE_FREQUENCY "PulseFrequency"
#define IUF_PATH_NUMPULSES "NumPulses"
#define IUF_PATH_PULSE_RAWPULSEAMPLITUDES "RawPulseAmplitudes"
#define IUF_PATH_PULSE_RAWPULSETIMES "RawPulseTimes"
#define IUF_PATH_PULSE_PULSEAMPLITUDES "PulseAmplitudes"
#define IUF_PATH_PULSE_PULSETIMES "PulseTimes"
#define IUF_PATH_RECEIVECHANNELMAPDICT "ReceiveChannelMaps"
#define IUF_PATH_RECEIVECHANNELMAP_MAP "Map"
#define IUF_PATH_RECEIVECHANNELMAP_NUMCHANNELS "NumChannels"
#define IUF_PATH_SOURCEDICT "Sources"
#define IUF_PATH_SOURCE_NUMSOURCES "NumSources"
#define IUF_PATH_SOURCE_LOCATIONLISTSIZE "Size"
#define IUF_PATH_SOURCE_LOCATIONLIST "Locations"
#define IUF_PATH_SOURCE_LOCATION "Location[%d]"
#define IUF_PATH_SOURCE_SOURCELABEL "SourceLabel"
#define IUF_PATH_SOURCE_SOURCETYPE "SourceType"
#define IUF_PATH_SOURCE_DELTATHETA "DeltaTheta"
#define IUF_PATH_SOURCE_DELTAPHI "DeltaPhi"
#define IUF_PATH_SOURCE_FNUMBER "FNumber"
#define IUF_PATH_SOURCE_STARTTHETA "StartTheta"
#define IUF_PATH_SOURCE_STARTPHI "StartPhi"
#define IUF_PATH_SOURCE_NUMPHILOCATIONS "NumPhiLocations"
#define IUF_PATH_SOURCE_NUMTHETALOCATIONS "NumThetaLocations"
#define IUF_PATH_TRANSDUCER "Transducer"
#define IUF_PATH_TRANSDUCER_ELEMENTLIST "Elements"
#define IUF_PATH_TRANSDUCER_ELEMENT "Elements[%d]"
#define IUF_PATH_TRANSDUCER_ELEMENTLIST_SIZE "Size"
#define IUF_PATH_TRANSDUCER_CENTERFREQUENCY "CenterFrequency"
#define IUF_PATH_TRANSDUCER_SHAPE "Shape"
#define IUF_PATH_TRANSDUCER_NAME "TransducerName"
#define IUF_PATH_TRANSMITAPODIZATIONDICT "TransmitApodizations"
#define IUF_PATH_TRANSMITAPODIZATION_NUMELEMENTS "NumElements"
#define IUF_PATH_TRANSMITAPODIZATION_APODIZATION "Apodization"
#define IUF_PATH_DEMODULATIONDICT "Demodulations"
#define IUF_PATH_DEMODULATION_SAMPLEFREQUENCY "SampleFrequency"
#define IUF_PATH_DEMODULATION_CENTERFREQUENCY "CenterFrequency"
#define IUF_PATH_DEMODULATION_NUMSAMPLESPERLINE "NumSamplesPerLine"
#define IUF_PATH_DEMODULATION_METHOD "Method"
#define IUF_PATH_DEMODULATION_TGC "Tgc"
#define IUF_PATH_DEMODULATION_PREFILTER "Prefilter"




#endif // _IUFIQFILESTRUCTURE_H
