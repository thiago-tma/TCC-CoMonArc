#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(CircularBuffer)
{
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullMemory);
}
