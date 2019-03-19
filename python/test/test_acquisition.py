import os
import shutil

from unittest import TestCase

import sys
if sys.version_info < (3, 0):
    from Python2Iuf import IufAcquisition
else:
    from Python3Iuf import IufAcquisition

class TestAcquisition(TestCase):
    def test_construction(self):
        # create with known parameter values
        # IufAcquisition(float speedOfSound, int date, char *description) {
        speedOfSound = 1234.5
        date = 12121994
        description = 'An important description for this Acquisition cannot be found!'
        a = IufAcquisition(speedOfSound, date, description)
        self.assertNotEqual(None,a)
        # verify values
        self.assertEqual(speedOfSound, a.speedOfSound)
        self.assertEqual(date, a.date)
        self.assertEqual(description, a.description)
        # destroy

