from unittest import TestCase
# from Python3Ius import *
# print("Creating transducer")
# t = ius2DTransducerCreate('my2DTransducer', IUS_LINE, 1230.2, 128)
# print("2DTransducer name = %s" % ius2DTransducerGetName(t) )
# print("Transducer name = %s" % iusTransducerGetName(t) )

import sys
if sys.version_info < (3, 0):
    from Python2Ius import *
else:
    from Python3Ius import *


class Test2DTransducer(TestCase):
    def test_construction(self):
        # create with known parameter values
        name = 'my2DTransducer'
        shape = IUS_LINE
        centerFrequency = 1230.2
        t = Ius2DTransducer(name, shape, centerFrequency, 128)
        self.assertEqual(name, iusTransducerGetName(t))
        self.assertEqual(shape, iusTransducerGetShape(t))
        # self.assertEqual(centerFrequency, ius2DTransducerGetCenterFrequency(t))


