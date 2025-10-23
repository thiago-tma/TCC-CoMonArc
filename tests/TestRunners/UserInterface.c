#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(UserInterface)
{
    RUN_TEST_CASE(UserInterface, CreateAndDestroy);
    RUN_TEST_CASE(UserInterface, InitializeGPIOModuleOnInitialization);
    RUN_TEST_CASE(UserInterface, SetPinsDirectionAndValueOnInitialization);
}