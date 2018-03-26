//
// Created by Ruijzendaal on 07/03/2018.
//

#include <stdio.h>
#include "ius.h"
#include <include/iusHLInput.h>
#include <include/iusHLInputFile.h>
#include <include/iusUtil.h>
#include <include/iusHLExperiment.h>
#include <util.h>
#include <include/iusError.h>
#include <unity_fixture.h>
#include "uniTests.h"
#include "iusTypes.h"



static void RunAllTests(void)
{
    RUN_TEST_GROUP(Misc);
    RUN_TEST_GROUP(InputfileTransducer);
    RUN_TEST_GROUP(InputfileReceiveSettings);
    RUN_TEST_GROUP(InputfileNode);
    RUN_TEST_GROUP(Inputfile);
    RUN_TEST_GROUP(Misc);
}

int main(int argc, const char * argv[])
{
    iusHLDiagDisable();
    return UnityMain(argc, argv, RunAllTests);
}