%module PythonIus

%{
#include <ius.h>
#include "iusAcquisitionADT.h"
#include "iusHistoryNodeADT.h"
#include "iusPatternListADT.h"

%}
#if 0
%include <ius2DNonParametricSource.h>
%include <ius2DParametricSource.h>
%include <ius2DSize.h>
%include <ius2DTransducer.h>
%include <ius2DTransducerElement.h>
%include <ius2DTransducerElementList.h>
%include <ius3DAngle.h>
%include <ius3DNonParametricSource.h>
%include <ius3DParametricSource.h>
%include <ius3DSize.h>
%include <ius3DTransducer.h>
%include <ius3DTransducerElement.h>
%include <ius3DTransducerElementList.h>
%include <iusBasicFunctions.h>
%include <iusData.h>
%include <iusDataStream.h>
%include <iusDemodulation.h>
%include <iusDemodulationDict.h>
%include <iusError.h>
%include <iusAcquisition.h>
%include <iusFile.h>
%include <iusFilter.h>
%include <iusFrame.h>
%include <iusFrameList.h>
%include <iusHDF5.h>
%include <iusHistoryNode.h>
%include <iusHistoryNodeList.h>
%include <iusInputFile.h>
%include <iusInputFileStructure.h>
%include <iusIqFile.h>
%include <iusIqFileStructure.h>
%include <iusNonParametricPulse.h>
%include <iusOffset.h>
%include <iusParameterDict.h>
%include <iusParametricPulse.h>
%include <iusPattern.h>
%include <iusPatternList.h>
%include <iusPatternListDict.h>
%include <iusIqPattern.h>
%include <iusIqPatternList.h>
%include <iusIqPatternListDict.h>
%include <iusPosition.h>
%include <iusPulse.h>
%include <iusPulseDict.h>
%include <iusReceiveChannelMap.h>
%include <iusReceiveChannelMapDict.h>
%include <iusReceiveSettings.h>
%include <iusReceiveSettingsDict.h>
%include <iusSource.h>
%include <iusSourceDict.h>
%include <iusTGC.h>
%include <iusTransducer.h>
%include <iusTransducerElement.h>
%include <iusTransmitApodization.h>
%include <iusTransmitApodizationDict.h>
%include <iusTypes.h>
%include <iusUtil.h>
%include <iusUuid.h>
%include <ius.h>
#endif

%include <iusError.h>
%include <iusPatternListADT.h>

%extend IusPatternList {
        IusPatternList() {
            int numPatterns = 1;
            iursd_t receiveSettingsDict = NULL;
            iurcmd_t receiveChannelMapDict = NULL;
            return iusPatternListCreate(numPatterns, receiveSettingsDict, receiveChannelMapDict);
        }

        ~IusPatternList() {
            iusPatternListDelete($self);
        }

};

%include <iusHistoryNodeADT.h>
%extend IusHistoryNode {             // Attach these functions to struct Vector
        IusHistoryNode(char *nodeType) {
            return iusHistoryNodeCreate(nodeType);
        }

        ~IusHistoryNode() {
            iusHistoryNodeDelete($self);
        }

        char *Type()
        {
            return iusHistoryNodeGetType($self);
        }

        int NumParams()
        {
            return iusHistoryNodeGetNumParams($self);
        }

        int NumParents()
        {
            return iusHistoryNodeGetNumParents($self);
        }

        char *__str__() {
            static char temp[256];
            sprintf(temp, "IusHistoryNode [%s]\n", $self->pType);
            return &temp[0];
        }
};

%include <iusAcquisitionADT.h>

%extend IusAcquisition {             // Attach these functions to struct Vector
        IusAcquisition(float speedOfSound, int date, char *description) {
            return iusAcquisitionCreate(speedOfSound,date,description);
        }
        ~IusAcquisition() {
            iusAcquisitionDelete($self);
        }

        char *__str__() {
            static char temp[256];
            sprintf(temp, "IusAcquisition [%g, %d, %s]\n", $self->speedOfSound, $self->date, $self->pDescription);
            return &temp[0];
        }
};