from unittest import TestCase

import sys
if sys.version_info < (3, 0):
    from Python2Iuf import *
else:
    from Python3Iuf import *


class TestErrorHandling(TestCase):
    def test_auto_report(self):
        iufErrorAutoReportSet(IUF_FALSE)
        with self.assertRaises(Exception) as context:
            inputFile = iufFileLoad(None);
        self.assertTrue('pFilename argument is NULL' in str(context.exception))

    def test_walk(self):
        iufErrorAutoReportSet(IUF_FALSE)
        with self.assertRaises(Exception) as context:
            inputFile = iufFileLoad(None);
            inputFile = iufFileLoad(None);

        self.assertTrue('pFilename argument is NULL' in str(context.exception))
        #

    def test_clearstack(self):
        iufErrorAutoReportSet(IUF_FALSE)
        iufErrorLogClear()
        self.assertEqual("", iufErrorString())
        try:
            inputFile = iufFileLoad(None);
        except Exception as ex:
            pass

        errorMsg = iufErrorString()
        self.assertTrue('pFilename argument is NULL' in errorMsg)
        iufErrorLogClear()
        self.assertEqual("", iufErrorString())
