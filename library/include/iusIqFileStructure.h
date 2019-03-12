//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iqFileStructure.h
//                  various ius types.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 * \brief The iq file structure serialization paths
 */

#ifndef _IUSIQFILESTRUCTURE_H
#define _IUSIQFILESTRUCTURE_H

#define IUS_PATH_ANGLE "Angle"
#define IUS_PATH_ANGLE_PHI "Phi"
#define IUS_PATH_ANGLE_THETA "Theta"
#define IUS_PATH_POSITION "Position"
#define IUS_PATH_POSITION_X "X"
#define IUS_PATH_POSITION_Y "Y"
#define IUS_PATH_POSITION_Z "Z"
#define IUS_PATH_SIZE "Size"
#define IUS_PATH_SIZE_X "SX"
#define IUS_PATH_SIZE_Y "SY"
#define IUS_PATH_SIZE_Z "SZ"

#define IUS_PATH_ACQUISITION "Acquisition"
#define IUS_PATH_ACQUISITION_DATE "Date"
#define IUS_PATH_ACQUISITION_DESCRIPTION "Description"
#define IUS_PATH_ACQUISITION_SPEEDOFSOUND "SpeedOfSound"
#define IUS_PATH_FRAMELIST "Frames"
#define IUS_PATH_FRAMELIST_FRAME "Frame[%d]"
#define IUS_PATH_FRAME_DATAINDEX "DataIndex"
#define IUS_PATH_FRAME_PATTERNLISTLABEL "PatternListLabel"
#define IUS_PATH_FRAME_TIME "Time"
#define IUS_PATH_FRAMELIST_SIZE "Size"
#define IUS_PATH_IUSVERSION "IusVersion"
#define IUS_PATH_NUMFRAMES "NumFrames"
//#define IUS_IQFILE_PATH_PATTERNLIST "Patterns"
#define IUS_PATH_PATTERNLISTDICT "Patterns"
#define IUS_PATH_PATTERNLIST_PATTERN "Pattern[%d]"
#define IUS_PATH_PATTERNLIST_SIZE "Size"
#define IUS_PATH_PATTERN_APODIZATIONLABEL "ApodizationLabel"
#define IUS_PATH_PATTERN_CHANNELMAPLABEL "ChannelMapLabel"
//#define IUS_IQFILE_PATH_PATTERNLIST_PATTERN_PATTERNLABEL "PatternLabel"
#define IUS_PATH_PATTERN_PULSELABEL "PulseLabel"
#define IUS_PATH_PATTERN_RECEIVESETTINGSLABEL "ReceiveSettingsLabel"
#define IUS_PATH_PATTERN_SOURCELABEL "SourceLabel"
#define IUS_PATH_PATTERN_DEMODULATIONLABEL "Demodulations"
#define IUS_PATH_PATTERN_TIMEINFRAME "TimeInFrame"
#define IUS_PATH_PULSEDICT "Pulses"
#define IUS_PATH_PULSE_NUMPULSEVALUES "NumPulseValues"
#define IUS_PATH_PULSE_PULSETYPE "PulseType"
#define IUS_PATH_PULSE_FREQUENCY "PulseFrequency"
#define IUS_PATH_NUMPULSES "NumPulses"
#define IUS_PATH_PULSE_RAWPULSEAMPLITUDES "RawPulseAmplitudes"
#define IUS_PATH_PULSE_RAWPULSETIMES "RawPulseTimes"
#define IUS_PATH_PULSE_PULSEAMPLITUDES "PulseAmplitudes"
#define IUS_PATH_PULSE_PULSETIMES "PulseTimes"
#define IUS_PATH_RECEIVECHANNELMAPDICT "ReceiveChannelMaps"
#define IUS_PATH_RECEIVECHANNELMAP_MAP "Map"
#define IUS_PATH_RECEIVECHANNELMAP_NUMCHANNELS "NumChannels"
#define IUS_PATH_SOURCEDICT "Sources"
#define IUS_PATH_SOURCE_NUMSOURCES "NumSources"
#define IUS_PATH_SOURCE_LOCATIONLISTSIZE "Size"
#define IUS_PATH_SOURCE_LOCATIONLIST "Locations"
#define IUS_PATH_SOURCE_LOCATION "Location[%d]"
#define IUS_PATH_SOURCE_SOURCELABEL "SourceLabel"
#define IUS_PATH_SOURCE_SOURCETYPE "SourceType"
#define IUS_PATH_SOURCE_DELTATHETA "DeltaTheta"
#define IUS_PATH_SOURCE_DELTAPHI "DeltaPhi"
#define IUS_PATH_SOURCE_FNUMBER "FNumber"
#define IUS_PATH_SOURCE_STARTTHETA "StartTheta"
#define IUS_PATH_SOURCE_STARTPHI "StartPhi"
#define IUS_PATH_SOURCE_NUMPHILOCATIONS "NumPhiLocations" 
#define IUS_PATH_SOURCE_NUMTHETALOCATIONS "NumThetaLocations" 
#define IUS_PATH_TRANSDUCER "Transducer"
#define IUS_PATH_TRANSDUCER_ELEMENTLIST "Elements"
#define IUS_PATH_TRANSDUCER_ELEMENT "Elements[%d]"
#define IUS_PATH_TRANSDUCER_ELEMENTLIST_SIZE "Size"
#define IUS_PATH_TRANSDUCER_CENTERFREQUENCY "CenterFrequency"
#define IUS_PATH_TRANSDUCER_SHAPE "Shape"
#define IUS_PATH_TRANSDUCER_NAME "TransducerName"
#define IUS_PATH_TRANSMITAPODIZATIONDICT "TransmitApodizations"
#define IUS_PATH_TRANSMITAPODIZATION_NUMELEMENTS "NumElements"
#define IUS_PATH_TRANSMITAPODIZATION_APODIZATION "Apodization" 
#define IUS_PATH_DEMODULATIONDICT "Demodulations" 
#define IUS_PATH_DEMODULATION_SAMPLEFREQUENCY "SampleFrequency"
#define IUS_PATH_DEMODULATION_CENTERFREQUENCY "CenterFrequency"
#define IUS_PATH_DEMODULATION_NUMSAMPLESPERLINE "NumSamplesPerLine"
#define IUS_PATH_DEMODULATION_METHOD "Method"
#define IUS_PATH_DEMODULATION_TGC "Tgc"
#define IUS_PATH_DEMODULATION_PREFILTER "Prefilter"




#endif // _IUSIQFILESTRUCTURE_H
