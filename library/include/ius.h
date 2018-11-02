/** ==============================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ==============================================================================
 *
 * \file ius.h
 * \brief Top level header file, include this file to access
 * all public ius function declarations.
 *
 *  ==============================================================================
 */


/**  \mainpage  IUS file IO library V3
*
* \section Overview
*
* The IUS library provides an API for reading and writing Ultrasound data and -derived and related data to HDF5 files.
*
* Conceptually, Ultrasound data is processed in an algorithm chain starting with beamforming, followed by
* property extraction such as signal intensity or velocity maps, and imaging steps such as compression and clutter
* reduction, etc. and or analysis steps such as tissue characterization. Additional data streams, such as ECG, are relevant
* enhancement data.
*
* This library facilitates such processing chains by providing abstract datatypes and functions for file IO.
* The HDF5 file format is used to store the following meta-data 
*    acquisition properties and settings
*    processing paramaters and settings
*    the history of all previous processing paramaters and settings.
* Since the meta-data is very small (10s of kBytes) typically compared to the data itself (100s of MBytes), storing all metadata of each 
* processing step remains relatively little overhead. In order to cope with various processing chain implementations and varsions of each 
* processing step is considered a #IusHistoryNode that has a "history" in the form of parents (a list of #IusHistoryNode s), node 
* specific properties (identified by the node type and optional algorithm parameters. In the following sections we will describe the implemented 
* Nodes types.  
*
* \section iusInputFile
* The image below shows the schematic structure of an #IusInputFile
* \image html Slide1.png
* The header contains a #IusFrameList where each #IusFrame consists of a reference to a #IusPatternList and a timestamp.
* The #IusPatternList is a list of #IusPattern. Which each contain references to 5 additional dictionaries: #IusSource,
* #IusReceiveSettings, #IusReceiveChannelMap, #IusTransmitApodization and #IusPulse. Together these 5 objects from each
* dictionary describe the pattern. See the subsections blow.
*
* In addition, an #IusInputFile contains a transducer description #IusTransducer and experiment description #IusExperiment.
*
* The data is stored in a 4D block of floats for each #IusPatternList with dimensions:
* numChannels x numSamplesPerLine x numPulses x numFrames. Note that this implies that these 4 dimension of each pattern in a patternList are constant. 
* This is checked by the library when constructign a patternList using #iusPatternListCreate() and #iusPatternListSet(). 
* 
* \subsection IusSourceDict
* The source dictionary contains a set of source types. A source does not have to contain a single point source, it can also contain multiple point sources, for example 
* to approximate a line source or for creating transversal waves. The locations of the sources are either specified in 2D or 3D space and either parametric or 
* non-paramteric, as such there are four options for an #IusSourceType.
*
* There is typically no need to define the generic #IusSource Type, instead the API provides functions for each of the four types separately.
* For example, to define a source in 3D space that approximates a circlular arc (described by startPhi, deltaPhi) 
* one would use:  #ius3DParametricSourceCreate(int numLocations, float fNumber, float deltaTheta, float startTheta, float deltaPhi, float startPhi).
* Please note that when one wants to compound individual insonifications from each point on the arc (i.e. fire individual pulses from each point source) then a
* pattern needs to be created for each of these insonifications and therefore also a source for each of these. In such a case each source then just contains a signle point.    
* A #IusSource is referenced by an #IusPattern using a string as a key in a dictionary.
* Typicall an empty dictionary is constructed once with #iusSourceDictCreate() and sources are added to the dictionary with #iusSourceDictSet() 
*  
* \subsection IusReceiveSettings
* The receive settings are a collection of parameters that describe the acquisition settings closely relating to the Analog to Digital Conversion of the data.
* It consists of: 
* - float _sampleFrequency_: the data acqisition rate 
* - float _startDelay_: The time between the 1st sample and the timeStamp of the pattern
* - int _numSamplesPerLine_: The number of samples acquired  
* - iutgc_t _TGC_: the Time Gain Control for these sampels: the tgc contains two arrays: time,gain describing or approximating the ADC amplifier function. An #IusTGC is 
* constructed using #iusTGCCreate() and filled by setting the time/gain pairs at each index using #iusTGCSet()
*
* An #IusPattern references an #IusReceiveSettings with a string in a dictionary. Typically an empty disctionary is constructed once using  #iusReceiveSettingsDictCreate()
* and #IusReceiveSettings are added to it with #iusReceiveSettingsDictSet(). The receive settings can be created using #iusReceiveSettingsCreate().
*
* \subsection IusReceiveChannelMap
*  THe receiveChannelMap contains a mapping table describing how the transducer elements are fit to the _numChannels_ receiving channels. Typically, not all elements 
*  can be captured simultaneously by an ultrasound system so _numChannels_ is typically smaller then _numElements_. The #IusReceiveChannelMap is _numChannels_ long and 
*  contains the indices of the transducer elements.
*  In case there is no channel mapping in place, a channel map needs to be created with a one-to-one mapping, i.e. an identity matrix of size numChannels==numElements.
*  An #IusReceiveChannelMap is constructed with #iusReceiveChannelMapCreate() and set with #iusReceiveChannelMapSetMap(). To add a #IusReceiveChannelMap to a dictionary use
*  #iusReceiveChannelMapDictSet() once you have a dictionary created with #iusReceiveChannelMapDictCreate()
* 
* \subsection IusPulse
* The shape of the emitted pulse at each source point is described either parametrically or as an array of time-amplitude pairs. In case of an #IusParametricPulse the 
* pulse is considered to consists of a number of sinusoid periods defined by an amplitude, frequency and cycleCount or the pulse is #IusNonParametricPulse it contains
* two float arrays of length numPulseValues named: pRawPulseAmplitudes and pRawPulseValues.
* The pulses are created with resp. #iusParametricPulseCreate() and #iusNonParametricPulseCreate() and both can be added to an #IusPulseDict using #iusPulseDictSet() once a 
* pulse dictionary is created using #iusPulseDictCreate(). 
*
* \subsection IusTransmitApodization
* The transmit Apodization describes the per-element gain of the transducer and is is allowed to change each #IusPattern. The #IusTransmitApodization is an array of floats
* of length _numElements_. I.e. there should be a gain for each transducer element.
* an #IusTransmitApodization is created with #iusTransmitApodizationCreate(), set with #iusTransmitApodizationSetApodization() and added to a dictionary with 
* #iusTransmitApodizationDictSet() 
*
* \section iusCWCFile
* 
* \section iusBModeFile
*
* \section iusColorDopplerFile
*
* etc...
* /
*
*
*/


/** \file
* \brief  various types for ius functions
*/



#ifndef IUS_H
#define IUS_H

#include <ius2DNonParametricSource.h>
#include <ius2DParametricSource.h>
#include <ius2DSize.h>
#include <ius2DTransducer.h>
#include <ius2DTransducerElement.h>
#include <ius2DTransducerElementList.h>
#include <ius3DAngle.h>
#include <ius3DNonParametricSource.h>
#include <ius3DParametricSource.h>
#include <ius3DSize.h>
#include <ius3DTransducer.h>
#include <ius3DTransducerElement.h>
#include <ius3DTransducerElementList.h>
#include <iusBasicFunctions.h>
#include <iusData.h>
#include <iusDataStream.h>
#include <iusError.h>
#include <iusAcquisition.h>
#include <iusFile.h>
#include <iusFrame.h>
#include <iusFrameList.h>
#include <iusHDF5.h>
#include <iusHistoryNode.h>
#include <iusHistoryNodeList.h>
#include <iusInputFile.h>
#include <iusInputFileStructure.h>
#include <iusNonParametricPulse.h>
#include <iusOffset.h>
#include <iusParameterDict.h>
#include <iusParametricPulse.h>
#include <iusPattern.h>
#include <iusPatternList.h>
#include <iusPatternListDict.h>
#include <iusPosition.h>
#include <iusPulse.h>
#include <iusPulseDict.h>
#include <iusReceiveChannelMap.h>
#include <iusReceiveChannelMapDict.h>
#include <iusReceiveSettings.h>
#include <iusReceiveSettingsDict.h>
#include <iusSource.h>
#include <iusSourceDict.h>
#include <iusTGC.h>
#include <iusTransducer.h>
#include <iusTransducerElement.h>
#include <iusTransmitApodization.h>
#include <iusTransmitApodizationDict.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusUuid.h>


#define  IUS_E_OK  0
#define  IUH_INVALID NULL
#define  IUS_INPUT_TYPE "IUSINPUTFILETYPE_V3"

int iusGetVersionMajor
(
  void
);

IUS_BOOL fileExists
(
  const char *fname
);

#endif //IUS_H
