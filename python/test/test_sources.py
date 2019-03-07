import os
import shutil

from unittest import TestCase

import sys
if sys.version_info < (3, 0):
    from Python2Ius import *
else:
    from Python3Ius import *

class TestSource(TestCase):
    def test_construction(self):
        # create with known parameter values
        # IusAcquisition(float speedOfSound, int date, char *description) {
        
		pos2D = ius2DPositionCreate(0.5, -1.0);
		source = ius2DNonParametricSourceCreate(1);
		ius2DNonParametricSourceSetPosition(source, pos2D, 0);

		sourceDict = iusSourceDictCreate();
		iusSourceDictSet(sourceDict, 'my2DNPSource',source);

		# now get the 2D position form this source in the dict
		mySource = iusSourceDictGet(sourceDict, 'my2DNPSource');
		my2DPos = ius2DNonParametricSourceGetPosition(mySource, 0);

		self.assertEqual(ius2DPositionCompare(pos2D, my2DPos), IUS_TRUE);
        # destroy

