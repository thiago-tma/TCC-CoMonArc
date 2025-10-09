#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(CircularBuffer)
{
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullMemory);
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullElementsNumber);
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullElementSize);
  RUN_TEST_CASE(CircularBuffer, CreateAndDestroy);
  RUN_TEST_CASE(CircularBuffer, BufferEmptyWhenCreated);
  RUN_TEST_CASE(CircularBuffer, ReadingBufferWhenEmptyReturnsFalse);
  RUN_TEST_CASE(CircularBuffer, BufferStoresElementAndReturnsTrue);
  RUN_TEST_CASE(CircularBuffer, BufferNotEmptyAfterWrite);
}
