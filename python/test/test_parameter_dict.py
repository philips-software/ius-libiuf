from unittest import TestCase

import sys
if sys.version_info < (3, 0):
    from Python2Ius import IusParameterDict
else:
    from Python3Ius import IusParameterDict


class TestParameterDict(TestCase):
    def test_construction(self):
        # create with known parameter values
        a = IusParameterDict()
        self.assertEqual(0, len(a))
        self.assertEqual([], a.keys())

    def test_set_get(self):
        # create with known parameter values
        key1 = '1stKey'
        key2 = '2ndKey'
        val1 = 'Mickey'
        val2 = 'Mouse'
        a = IusParameterDict()
        self.assertEqual(0, len(a))
        a[key1] = val1
        self.assertEqual(1, len(a))
        self.assertTrue(key1 in a.keys())
        a[key2] = val2
        self.assertEqual(2, len(a))
        self.assertTrue(key1 in a.keys())
        self.assertEqual(val1, a[key1])
        self.assertEqual(val2, a[key2])
