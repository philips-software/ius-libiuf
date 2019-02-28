# TEST(IusErrorHandling, testIusErrorAutoReport)
# {
#     iusErrorAutoReport(IUS_TRUE);
# FILE *fp = fopen(pErrorFilename, "w+");
# long filePos = ftell(fp);
# iusErrorSetStream(fp);
# iuf_t inputFile = iusFileLoad(NULL);
# TEST_ASSERT(inputFile == IUFI_INVALID);
# TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));
# TEST_ASSERT_EQUAL(1,iusErrorGetCount());
#
# // Ok, let's try again, with auto-reporting disabled
# iusErrorAutoReport(IUS_FALSE);
# filePos = ftell(fp);
# inputFile = iusFileLoad(NULL);
# TEST_ASSERT(inputFile == IUFI_INVALID);
# TEST_ASSERT_EQUAL(filePos,ftell(fp));
# TEST_ASSERT_EQUAL(2,iusErrorGetCount());
# fclose(fp);
# iusFileDelete(inputFile);
# }
import io
import os
from unittest import TestCase

import sys
if sys.version_info < (3, 0):
    from Python2Ius import *
else:
    from Python3Ius import *


class TestErrorHandling(TestCase):
    def test_auto_report(self):
        iusErrorAutoReportSet(IUS_FALSE)
        with self.assertRaises(Exception) as context:
            inputFile = iusFileLoad(None);
        self.assertTrue('pFilename argument is NULL' in str(context.exception))

    def test_walk(self):
        iusErrorAutoReportSet(IUS_FALSE)
        with self.assertRaises(Exception) as context:
            inputFile = iusFileLoad(None);
            inputFile = iusFileLoad(None);

        self.assertTrue('pFilename argument is NULL' in str(context.exception))
        #

    def test_clearstack(self):
        iusErrorAutoReportSet(IUS_FALSE)
        iusErrorLogClear()
        self.assertEqual("", iusErrorString())
        try:
            inputFile = iusFileLoad(None);
        except Exception as ex:
            pass

        errorMsg = iusErrorString()
        self.assertTrue('pFilename argument is NULL' in errorMsg)
        iusErrorLogClear()
        self.assertEqual("123", iusErrorString())
