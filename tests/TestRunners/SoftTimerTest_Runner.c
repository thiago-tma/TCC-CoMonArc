#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(SoftTimer)
{
    RUN_TEST_CASE(SoftTimer, CreateAndDestroySoftwareTimer);
    RUN_TEST_CASE(SoftTimer, TimerPeriodMustNotBeZeroInInitialization);
}