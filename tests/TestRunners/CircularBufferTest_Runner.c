#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(CircularBuffer)
{
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullMemory);
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullMemory);
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullSize);
  RUN_TEST_CASE(CircularBuffer, SucessfulInicializationReturnsTrue);
}
