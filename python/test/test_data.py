from unittest import TestCase

import sys
import numpy as np
if sys.version_info < (3, 0):
    from Python2Iuf import *
else:
    from Python3Iuf import *


def saveFrames(inputFile, label, numFrames):
    status=0
    frame = iufInputFileFrameCreate(inputFile, label);
    offset = iufOffsetCreate();
    array = np.ones(frame.size)

    for i in range(numFrames):
        array.fill(i*i)
        iufDataFill(frame, array)
        offset.t = i;
        status |= iufInputFileFrameSave(inputFile, label, frame, offset)

    iufOffsetDelete(offset)
    iufDataDelete(frame)
    return status


class TestData(TestCase):
    # def test_construction(self):
    #     numChannels = 2
    #     numSamplesPerLine = 3
    #     numPulses = 4
    #     # Test double to float data interface
    #
    #     # simulate ifh = iufInputFileFrameCreate()
    #     frameSize = numChannels*numSamplesPerLine*numPulses
    #     data = iufDataCreate(frameSize)
    #     self.assertNotEqual(None, data)
    #     self.assertEqual(frameSize, len(data))
    #     # data kan now be filled in Python space
    #     # iufInputFileSaveFrame(ifh, frame, offset)

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
        # *  fill array and hand over to iuf
        #    using the iufInputFileSave routine
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
        frame = iufInputFileFrameCreate(ifh, dopplerLabel)
        self.assertNotEqual(None,frame)
        status = iufInputFileNodeSave(ifh)
        self.assertEqual(0, status)
        status = saveFrames(ifh, dopplerLabel, numFrames)
        self.assertEqual(0, status)
        iufInputFileClose(ifh)