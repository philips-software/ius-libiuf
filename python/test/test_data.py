from unittest import TestCase

import sys
if sys.version_info < (3, 0):
    from Python2Ius import *
else:
    from Python3Ius import *


def saveFrames(inputFile, label, numFrames):
    status=0
    frame = iusInputFileFrameCreate(inputFile, label);
    offset = iusOffsetCreate();

    for i in range(numFrames):
        dgFillData(frame, 1 + i * 1.0)
        # offset->t = i;
        status |= iusInputFileFrameSave(inputFile, label, frame, offset)

    iusOffsetDelete(offset)
    iusDataDelete(frame)
    return status


class TestData(TestCase):
    # def test_construction(self):
    #     numChannels = 2
    #     numSamplesPerLine = 3
    #     numPulses = 4
    #     # Test double to float data interface
    #
    #     # simulate ifh = iusInputFileFrameCreate()
    #     frameSize = numChannels*numSamplesPerLine*numPulses
    #     data = iusDataCreate(frameSize)
    #     self.assertNotEqual(None, data)
    #     self.assertEqual(frameSize, len(data))
    #     # data kan now be filled in Python space
    #     # iusInputFileSaveFrame(ifh, frame, offset)

    def test_usage_scenario(self):
        # As a Python programmer
        # I want to have a transparent interface
        # and do not have to know about the internals of the
        # C-API.
        #
        # *  So: Create file
        # *  Create Frame (using the meta data in the file like
        #    numSamplesPerline,numChannels and number of elements)
        # *  createFrame returns a numpyArray in Python Space.
        # *  fill array and hand over to ius
        #    using the iusInputFileSave routine
        #
        testFileName = "TestData.test_usage_scenario.hdf5"
        dopplerLabel = "doppler"
        numFrames = 10
        numSamplesPerLine = 10
        numChannels = 8
        bmodeLabel = "bmode"
        status = 0

        ifh = dgGenerateInputFile(testFileName, "S5-1", dopplerLabel, numFrames, numSamplesPerLine, numChannels)
        self.assertNotEqual(None, ifh)
        frame = iusInputFileFrameCreate(ifh, dopplerLabel)
        self.assertNotEqual(None,frame)
        status = iusInputFileNodeSave(ifh)
        self.assertEqual(0, status)
        status = saveFrames(ifh, dopplerLabel, numFrames)
        self.assertEqual(0, status)
        iusInputFileClose(ifh)

# status = dgInputFileAddGeneratedData(ifh, bmodeLabel, numSamplesPerLine, numChannels)
        # self.assertEqual(0, status)
