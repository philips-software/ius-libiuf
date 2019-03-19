from unittest import TestCase
# from Python3Iuf import *
# print("Creating transducer")
# t = iuf2DTransducerCreate('my2DTransducer', IUF_LINE, 1230.2, 128)
# print("2DTransducer name = %s" % iuf2DTransducerGetName(t) )
# print("Transducer name = %s" % iufTransducerGetName(t) )

import sys
if sys.version_info < (3, 0):
    from Python2Iuf import *
else:
    from Python3Iuf import *


class Test2DTransducer(TestCase):
    def test_construction(self):
        # create with known parameter values
        name = 'my2DTransducer'
        shape = IUF_LINE
        centerFrequency = 1230.2
        t = Iuf2DTransducer(name, shape, centerFrequency, 128)
        self.assertEqual(name, iufTransducerGetName(t))
        self.assertEqual(shape, iufTransducerGetShape(t))
        self.assertAlmostEqual(centerFrequency, iuf2DTransducerGetCenterFrequency(t), places=4)


