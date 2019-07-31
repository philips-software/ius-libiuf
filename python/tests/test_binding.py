import unittest
from libiuf import *


class TestGetMajorVersion(unittest.TestCase):
    def setup(self):
        pass

    def teardown(self):
        pass

    def testListNew(self):
        v = iufGetVersionMajor()
        self.assertTrue(v != 0)


if __name__ == '__main__':
    unittest.main()
