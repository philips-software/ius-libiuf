//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusTypes.h
//                  various ius types.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 * \brief The input file structure serialization paths
 */

#ifndef _IUSINPUTFILESTRUCTURE_H
#define _IUSINPUTFILESTRUCTURE_H

#define IUS_INPUTFILE_PATH_ANGLE "Angle"
#define IUS_INPUTFILE_PATH_ANGLE_PHI "Phi"
#define IUS_INPUTFILE_PATH_ANGLE_THETA "Theta"
#define IUS_INPUTFILE_PATH_POSITION "Position"
#define IUS_INPUTFILE_PATH_POSITION_X "X"
#define IUS_INPUTFILE_PATH_POSITION_Y "Y"
#define IUS_INPUTFILE_PATH_POSITION_Z "Z"
#define IUS_INPUTFILE_PATH_SIZE "Size"
#define IUS_INPUTFILE_PATH_SIZE_X "SX"
#define IUS_INPUTFILE_PATH_SIZE_Y "SY"
#define IUS_INPUTFILE_PATH_SIZE_Z "SZ"

#define IUS_INPUTFILE_PATH_ACQUISITION "Acquisition"
#define IUS_INPUTFILE_PATH_ACQUISITION_DATE "Date"
#define IUS_INPUTFILE_PATH_ACQUISITION_DESCRIPTION "Description"
#define IUS_INPUTFILE_PATH_ACQUISITION_SPEEDOFSOUND "SpeedOfSound"
#define IUS_INPUTFILE_PATH_FRAMELIST "Frames"
#define IUS_INPUTFILE_PATH_FRAMELIST_FRAME "Frame[%d]"
#define IUS_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX "DataIndex"
#define IUS_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTLABEL "PatternListLabel"
#define IUS_INPUTFILE_PATH_FRAMELIST_FRAME_TIME "Time"
#define IUS_INPUTFILE_PATH_FRAMELIST_SIZE "Size"
#define IUS_INPUTFILE_PATH_IUSVERSION "IusVersion"
#define IUS_INPUTFILE_PATH_NUMFRAMES "NumFrames"
//#define IUS_INPUTFILE_PATH_PATTERNLIST "Patterns"
#define IUS_INPUTFILE_PATH_PATTERNLISTDICT "Patterns"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN "Pattern[%d]"
#define IUS_INPUTFILE_PATH_PATTERNLIST_SIZE "Size"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL "ApodizationLabel"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL "ChannelMapLabel"
//#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PATTERNLABEL "PatternLabel"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL "PulseLabel"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_RECEIVESETTINGSLABEL "ReceiveSettingsLabel"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL "SourceLabel"
#define IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME "TimeInFrame"
#define IUS_INPUTFILE_PATH_PULSEDICT "Pulses"
#define IUS_INPUTFILE_PATH_PULSE_NUMPULSEVALUES "NumPulseValues"
#define IUS_INPUTFILE_PATH_PULSE_PULSETYPE "PulseType"
#define IUS_INPUTFILE_PATH_PULSE_FREQUENCY "PulseFrequency"
#define IUS_INPUTFILE_PATH_NUMPULSES "NumPulses"
#define IUS_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES "RawPulseAmplitudes"
#define IUS_INPUTFILE_PATH_PULSE_RAWPULSETIMES "RawPulseTimes"
#define IUS_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES "PulseAmplitudes"
#define IUS_INPUTFILE_PATH_PULSE_PULSETIMES "PulseTimes"
#define IUS_INPUTFILE_PATH_RECEIVECHANNELMAPDICT "ReceiveChannelMaps"
#define IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP "Map"
#define IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS "NumChannels"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGSDICT "ReceiveSettings"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_LABEL "ReceiveSettingsLabel"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE "NumSamplesPerLine"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY "SampleFrequency"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY "StartDelay"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC "TGC"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS "Gains"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES "Times"
#define IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES "NumTGCValues"
#define IUS_INPUTFILE_PATH_SOURCEDICT "Sources"
#define IUS_INPUTFILE_PATH_SOURCE_LISTSIZE "Size"
#define IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST "Locations"
#define IUS_INPUTFILE_PATH_SOURCE_LOCATION "Location[%d]"
#define IUS_INPUTFILE_PATH_SOURCE_SOURCELABEL "SourceLabel"
#define IUS_INPUTFILE_PATH_SOURCE_SOURCETYPE "SourceType"
#define IUS_INPUTFILE_PATH_SOURCE_DELTATHETA "DeltaTheta"
#define IUS_INPUTFILE_PATH_SOURCE_DELTAPHI "DeltaPhi"
#define IUS_INPUTFILE_PATH_SOURCE_FNUMBER "FNumber"
#define IUS_INPUTFILE_PATH_SOURCE_STARTTHETA "StartTheta"
#define IUS_INPUTFILE_PATH_SOURCE_STARTPHI "StartPhi"
#define IUS_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS "NumPhiLocations" 
#define IUS_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS "NumThetaLocations" 
#define IUS_INPUTFILE_PATH_TRANSDUCER "Transducer"
#define IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST "Elements"
#define IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENT "Elements[%d]"
#define IUS_INPUTFILE_PATH_TRANSDUCER_ELEMENTLIST_SIZE "Size"
#define IUS_INPUTFILE_PATH_TRANSDUCER_CENTERFREQUENCY "CenterFrequency"
#define IUS_INPUTFILE_PATH_TRANSDUCER_SHAPE "Shape"
#define IUS_INPUTFILE_PATH_TRANSDUCER_NAME "TransducerName"
#define IUS_INPUTFILE_PATH_TRANSMITAPODIZATIONDICT "TransmitApodizations"
#define IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_NUMELEMENTS "NumElements"
#define IUS_INPUTFILE_PATH_TRANSMITAPODIZATION_APODIZATION "Apodization" 


#endif // _IUSINPUTFILESTRUCTURE_H
