#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(CircularBuffer);
  RUN_TEST_GROUP(SystemClock);
  RUN_TEST_GROUP(SoftTimer);      /* Depende do SystemClock */ 
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}