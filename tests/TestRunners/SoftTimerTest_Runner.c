#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(SoftTimer)
{
    RUN_TEST_CASE(SoftTimer, CreateAndDestroySoftwareTimer);
    RUN_TEST_CASE(SoftTimer, TimerPeriodMustNotBeZeroInInitialization);
    RUN_TEST_CASE(SoftTimer, CheckTimerReturnsFalseWhenItsNotTime);
    RUN_TEST_CASE(SoftTimer, CheckTimerReturnsTrueWhenTimerExpires);
    RUN_TEST_CASE(SoftTimer, CheckTimerReturnsFalseJustAfterReturningTrue);
    RUN_TEST_CASE(SoftTimer, CheckTimerReturnsTrueAgainAfterTimePeriod);
    RUN_TEST_CASE(SoftTimer, CheckTimeOnSystemTimeOverflow);
    RUN_TEST_CASE(SoftTimer, ResetingTimerStartsOverCounting);
    RUN_TEST_CASE(SoftTimer, DestroyTimer);
}