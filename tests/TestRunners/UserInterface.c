#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(UserInterface)
{
    RUN_TEST_CASE(UserInterface, CreateAndDestroy);
    RUN_TEST_CASE(UserInterface, InitializeGPIOModuleOnInitialization);
    RUN_TEST_CASE(UserInterface, SetPinsDirectionAndValueOnInitialization);
    RUN_TEST_CASE(UserInterface, BlinkLEDOneTime);
    RUN_TEST_CASE(UserInterface, CallBlinkLedWithZeroRepetitionsTurnsOffActuator);
    RUN_TEST_CASE(UserInterface, CallBlinkLedWhileAlreadyBlinkingOverwritesLastBlinkCommand);
    RUN_TEST_CASE(UserInterface, BlinkBuzzerThreeTimes500msOnAnd100msOff);
    RUN_TEST_CASE(UserInterface, DebouncedButtonActivatesCallback);
    RUN_TEST_CASE(UserInterface, ButtonPressWithoutCallbackHasNoEffect);
    RUN_TEST_CASE(UserInterface, ButtonBounceIsIgnored);
    RUN_TEST_CASE(UserInterface, PressButtonTwice);
}