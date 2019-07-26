import os
import shutil

from unittest import TestCase

import sys

if sys.version_info < (3, 0):
    from Python2Iuf import *
else:
    from Python3Iuf import *


class TestSource(TestCase):
    def test_construction(self):
        # create with known parameter values
        # IufAcquisition(float speedOfSound, int date, char *description) {

        pos2D = iuf2DPositionCreate(0.5, -1.0)
        source = iuf2DNonParametricSourceCreate(1)
        iuf2DNonParametricSourceSetPosition(source, pos2D, 0)

        sourceDict = iufSourceDictCreate();
        iufSourceDictSet(sourceDict, 'my2DNPSource', source)

        # now get the 2D position form this source in the dict
        mySource = iufSourceDictGet(sourceDict, 'my2DNPSource')
        my2DPos = iuf2DNonParametricSourceGetPosition(mySource, 0)

        self.assertEqual(iuf2DPositionCompare(pos2D, my2DPos), IUF_TRUE)
    # destroy
