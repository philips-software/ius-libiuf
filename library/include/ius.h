//
// Created by Ruijzendaal on 08/03/2018.
//
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
