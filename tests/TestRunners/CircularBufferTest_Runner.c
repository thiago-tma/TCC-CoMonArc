#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(CircularBuffer)
{
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullMemory);
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullMemory);
  RUN_TEST_CASE(CircularBuffer, BadInitializationNullSize);
  RUN_TEST_CASE(CircularBuffer, SucessfulInicializationReturnsTrue);
  RUN_TEST_CASE(CircularBuffer, IsEmptyAfterCreate);
  RUN_TEST_CASE(CircularBuffer, ReadWhileEmptyReturnsFalse);
  RUN_TEST_CASE(CircularBuffer, WriteWhenEmptyReturnsTrue);
  RUN_TEST_CASE(CircularBuffer, ShowsNotEmptyAfterWrite);
  RUN_TEST_CASE(CircularBuffer, ReturnTrueOnReadAfterWrite);
  RUN_TEST_CASE(CircularBuffer, WriteAndReadTheSameRandomByte);
  RUN_TEST_CASE(CircularBuffer, BufferEmptyAgainAfterWriteAndRead);
  RUN_TEST_CASE(CircularBuffer, ReinitializeBufferOnSecondCreate);
  RUN_TEST_CASE(CircularBuffer, ReinitializeBufferOnDestroy);
  RUN_TEST_CASE(CircularBuffer, Write2BytesAndRead2Bytes);
  RUN_TEST_CASE(CircularBuffer, ShowSpaceRemainingAfterWrite);
  RUN_TEST_CASE(CircularBuffer, ShowSpaceRemainingAfter4WriteAnd2Read);
}
