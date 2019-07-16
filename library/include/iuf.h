/**  
 * \file iuf.h
 * \brief Top level header file, include this file to access
 * all public iuf function declarations.
 */


/**  \mainpage  IUF file IO library V3
*
* \section Overview
*
* The IUF library provides an API for reading and writing Ultrasound data and -derived and related data to HDF5 files.
*
* Conceptually, Ultrasound data is processed in an algorithm chain starting with beamforming, followed by
* property extraction such as signal intensity or velocity maps, and imaging steps such as compression and clutter
* reduction, etc. and or analysis steps such as tissue characterization. Additional data streams, such as ECG, are relevant
* enhancement data.
*
* This library facilitates such processing chains by providing abstract datatypes and functions for file IO.
* The HDF5 file format is used to store the following meta-data 
*    acquisition properties and settings
*    processing parameters and settings
*    the history of all previous processing parameters and settings.
* Since the meta-data is very small (10s of kBytes) typically compared to the data itself (100s of MBytes), storing all metadata of each 
* processing step remains relatively little overhead. In order to cope with various processing chain implementations and versions of each
* processing step is considered a #IufHistoryNode that has a "history" in the form of parents (a list of #IufHistoryNode s), node
* specific properties (identified by the node type and optional algorithm parameters). In the following sections we will describe the implemented
* Nodes types.  
*
* \section iufInputFile
* The image below shows the schematic structure of an #IufInputFile
* \image html Slide1.png
* The header contains a #IufFrameList where each #IufFrame consists of a reference to a #IufPatternList and a timestamp.
* The #IufPatternList is a list of #IufPattern. Which each contain references to 5 additional dictionaries: #IufSource,
* #IufReceiveSettings, #IufReceiveChannelMap, #IufTransmitApodization and #IufPulse. Together these 5 objects from each
* dictionary describe the pattern. See the subsections blow.
*
* In addition, an #IufInputFile contains a transducer description #IufTransducer and the acquistion method #IufAcquisition.
*
* The data is stored in a 4D block of floats for each #IufPatternList with dimensions:
* numChannels x numSamplesPerLine x numPulses x numFrames. Note that this implies that these 4 dimension of each pattern in a patternList are constant. 
* This is checked by the library when constructing a patternList using #iufPatternListCreate() and #iufPatternListSet().
* 
* \subsection IufSourceDict
* The source dictionary contains a set of source types. A source does not have to contain a single point source, it can also contain multiple point sources, for example 
* to approximate a line source or for creating transversal waves. The locations of the sources are either specified in 2D or 3D space and either parametric or 
* non-paramteric, as such there are four options for an #IufSourceType.
*
* There is typically no need to define the generic #IufSource Type, instead the API provides functions for each of the four types separately.
* For example, to define a source in 3D space that approximates a circlular arc (described by startPhi, deltaPhi) 
* one would use:  #iuf3DParametricSourceCreate().
* Please note that when one wants to compound individual insonifications from each point on the arc (i.e. fire individual pulses from each point source) then a
* pattern needs to be created for each of these insonifications and therefore also a source for each of these. In such a case each source then just contains a signle point.    
* A #IufSource is referenced by an #IufPattern using a string as a key in a dictionary.
* Typicall an empty dictionary is constructed once with #iufSourceDictCreate() and sources are added to the dictionary with #iufSourceDictSet()
*  
* \subsection IufReceiveSettings
* The receive settings are a collection of parameters that describe the acquisition settings closely relating to the Analog to Digital Conversion of the data.
* It consists of: 
* - float _sampleFrequency_: the data acqisition rate 
* - float _startDelay_: The time between the 1st sample and the timeStamp of the pattern
* - int _numSamplesPerLine_: The number of samples acquired  
* - iutgc_t _TGC_: the Time Gain Control for these sampels: the tgc contains two arrays: time,gain describing or approximating the ADC amplifier function. An #IufTGC is
* constructed using #iufTGCCreate() and filled by setting the time/gain pairs at each index using #iufTGCSet()
*
* An #IufPattern references an #IufReceiveSettings with a string in a dictionary. Typically an empty disctionary is constructed once using  #iufReceiveSettingsDictCreate()
* and #IufReceiveSettings are added to it with #iufReceiveSettingsDictSet(). The receive settings can be created using #iufReceiveSettingsCreate().
*
* \subsection IufReceiveChannelMap
*  THe receiveChannelMap contains a mapping table describing how the transducer elements are fit to the _numChannels_ receiving channels. Typically, not all elements 
*  can be captured simultaneously by an ultrasound system so _numChannels_ is typically smaller then _numElements_. The #IufReceiveChannelMap is _numChannels_ long and
*  contains the indices of the transducer elements.
*  In case there is no channel mapping in place, a channel map needs to be created with a one-to-one mapping, i.e. an identity matrix of size numChannels==numElements.
*  An #IufReceiveChannelMap is constructed with #iufReceiveChannelMapCreate() and set with #iufReceiveChannelMapSetMap(). To add a #IufReceiveChannelMap to a dictionary use
*  #iufReceiveChannelMapDictSet() once you have a dictionary created with #iufReceiveChannelMapDictCreate()
* 
* \subsection IufPulse
* The shape of the emitted pulse at each source point is described either parametrically or as an array of time-amplitude pairs. In case of an #IufParametricPulse the
* pulse is considered to consists of a number of sinusoid periods defined by an amplitude, frequency and cycleCount or the pulse is #IufNonParametricPulse it contains
* two float arrays of length numPulseValues named: pRawPulseAmplitudes and pRawPulseValues.
* The pulses are created with resp. #iufParametricPulseCreate() and #iufNonParametricPulseCreate() and both can be added to an #IufPulseDict using #iufPulseDictSet() once a
* pulse dictionary is created using #iufPulseDictCreate().
*
* \subsection IufTransmitApodization
* The transmit Apodization describes the per-element gain of the transducer and is is allowed to change each #IufPattern. The #IufTransmitApodization is an array of floats
* of length _numElements_. I.e. there should be a gain for each transducer element.
* an #IufTransmitApodization is created with #iufTransmitApodizationCreate(), set with #iufTransmitApodizationSetApodization() and added to a dictionary with
* #iufTransmitApodizationDictSet()
*
* \section iufCWCFile
* 
* \section iufBModeFile
*
* \section iufColorDopplerFile
*
* etc...
* /
*
*
*/


/** \file
* \brief  various types for iuf functions
*/



#ifndef IUF_H
#define IUF_H

#include <iuf2DNonParametricSource.h>
#include <iuf2DParametricSource.h>
#include <iuf2DSize.h>
#include <iuf2DTransducer.h>
#include <iuf2DTransducerElement.h>
#include <iuf2DTransducerElementList.h>
#include <iuf3DAngle.h>
#include <iuf3DNonParametricSource.h>
#include <iuf3DParametricSource.h>
#include <iuf3DSize.h>
#include <iuf3DTransducer.h>
#include <iuf3DTransducerElement.h>
#include <iuf3DTransducerElementList.h>
#include <iufBasicFunctions.h>
#include <iufCompoundFile.h>
#include <iufCompoundFileStructure.h>
#include <iufData.h>
#include <iufDataStream.h>
#include <iufDemodulation.h>
#include <iufDemodulationDict.h>
#include <iufError.h>
#include <iufAcquisition.h>
#include <iufFile.h>
#include <iufFilter.h>
#include <iufFrame.h>
#include <iufFrameList.h>
#include <iufHDF5.h>
#include <iufHistoryNode.h>
#include <iufHistoryNodeList.h>
#include <iufInputFile.h>
#include <iufInputFileStructure.h>
#include <iufIqFile.h>
#include <iufIqFileStructure.h>
#include <iufNonParametricPulse.h>
#include <iufOffset.h>
#include <iufParameterDict.h>
#include <iufParametricPulse.h>
#include <iufPattern.h>
#include <iufPatternList.h>
#include <iufPatternListDict.h>
#include <iufIqPattern.h>
#include <iufIqPatternList.h>
#include <iufIqPatternListDict.h>
#include <iufPosition.h>
#include <iufPulse.h>
#include <iufPulseDict.h>
#include <iufReceiveChannelMap.h>
#include <iufReceiveChannelMapDict.h>
#include <iufReceiveSettings.h>
#include <iufReceiveSettingsDict.h>
#include <iufSource.h>
#include <iufSourceDict.h>
#include <iufTGC.h>
#include <iufTransducer.h>
#include <iufTransducerElement.h>
#include <iufTransmitApodization.h>
#include <iufTransmitApodizationDict.h>
#include <iufTypes.h>
#include <iufUtil.h>
#include <iufUuid.h>


#define  IUF_E_OK  0
#define  IUH_INVALID NULL
#define  IUF_INPUT_TYPE "IUFINPUTFILETYPE_V3"
#define  IUF_IQ_TYPE "IUFIQFILETYPE_V3"
#define  IUF_COMPOUND_TYPE "IUFCOMPOUNDFILETYPE_V3"

int iufGetVersionMajor
(
  void
);

IUF_BOOL fileExists
(
  const char *fname
);

#endif //IUF_H
