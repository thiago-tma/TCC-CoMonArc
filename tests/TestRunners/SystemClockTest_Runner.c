#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(SystemClock)
{
  RUN_TEST_CASE(SystemClock, TimeIsZeroAfterCreate);
  RUN_TEST_CASE(SystemClock, TimeIncreasedAfterSystemCallback);
  RUN_TEST_CASE(SystemClock, MultipleInitializatonsDoNotChangeTime);
  RUN_TEST_CASE(SystemClock, TimeResetAfterDestroy);
  RUN_TEST_CASE(SystemClock, TimeReadIsInterruptProof);
  RUN_TEST_CASE(SystemClock, ReturnTrueWhenSuccessfullyRegisteringCallBackFunction);
  RUN_TEST_CASE(SystemClock, DestroyingModuleResetsRegisteredCallBacks);
  RUN_TEST_CASE(SystemClock, RunCallBackFunctionOnSystemTick);
  RUN_TEST_CASE(SystemClock, RunMultipleCallBackFunctionOnSystemTick);
  RUN_TEST_CASE(SystemClock, AddCallBackReturnsMinus1IfNoSlotAvailable);
  RUN_TEST_CASE(SystemClock, RemoveCallBack);
}
