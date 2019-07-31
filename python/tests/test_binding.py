import unittest
from libiuf import *


class TestGetMajorVersion(unittest.TestCase):
    def setup(self):
        pass

    def teardown(self):
        pass

    def testListNew(self):
        ma = iufGetVersionMajor()
        mi = iufGetVersionMinor()
        p = iufGetVersionPatch()
        assembled_version = "{0}.{1}.{2}".format(ma, mi, p)
        version = iufGetVersion()
        self.assertEqual(version, assembled_version)


if __name__ == '__main__':
    unittest.main()
