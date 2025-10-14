#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(SystemClock)
{
  RUN_TEST_CASE(SystemClock, TimeIsZeroAfterCreate);
  RUN_TEST_CASE(SystemClock, TimeIncreasedAfterSystemCallback);
  RUN_TEST_CASE(SystemClock, TimeResetAfterDestroy);
  RUN_TEST_CASE(SystemClock, TimeReadIsInterruptProof);
}
