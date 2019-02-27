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

# def test_spike(self):
    #     import ctypes
    #     libc = ctypes.CDLL(None)
    #     old_stdout = sys.stdout
    #     f = io.StringIO()
    #     with stdout_redirector_1(f):
    #         print('foobar')
    #         print(12)
    #         libc.puts(b'this comes from C')
    #         os.system('echo and this is from echo')
    #         # sys.stderr.write
    #
    #     sys.stdout = old_stdout
    #     print('\n=>>>> Got stdout:\n"{0}"'.format(f.getvalue()))
    #
    #
    #     old_stdout = sys.stdout
    #     f = io.BytesIO()
    #     with stdout_redirector(f):
    #         print('foobar')
    #         print(12)
    #         libc.puts(b'this comes from C')
    #         os.system('echo and this is from echo')
    #         # sys.stderr.write
    #
    #     sys.stdout = old_stdout
    #     print('\n=>>>> Got stdout:\n"{0}"'.format(f.getvalue()))
    #
    #
    #     c = Capture()
    #     c.start()
    #     os.system('echo 10')
    #     x = c.stop()
