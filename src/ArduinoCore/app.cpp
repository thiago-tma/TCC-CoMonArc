#include <Arduino.h>

#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>

#include <Logger/include/Logger.h>
#include <Logger/include/log_api.h>
#include <Transmitter/include/Transmitter.h>
#include <HAL/UART.h>

#include <YawController.h>
#include <Magnetometer.h>
#include <UserInterface.h>
#include <CurrentSensor.h>

#include <SoftTimer.h>

#define SYSTEM_LOOP_PERIOD_US 50000

static bool yawControllerRunning = false;
static SoftTimer loopTimer;

static void buttonFunction (void)
{
    if (yawControllerRunning)
    {
        YawController_Destroy();
        yawControllerRunning = false;
    }
    else
    {
        YawController_Create();
        yawControllerRunning = true;
    }
}

static void UART_Transmit  (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    (void) origin; (void) level;
    HAL_UART_SendByte(0xAA);
    HAL_UART_SendByte(messageID);
    HAL_UART_SendByte(payloadSize);
    if (payloadSize > 0)
    {
        HAL_UART_SendPayload(payload, payloadSize);
    }
}

static void ErrorLoop (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    static bool errorState = false;
    if (level != LOG_LEVEL_ERROR || errorState == true) return;
    errorState = true;

    YawController_Destroy();
    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_ERROR, true, true); /*Enable only errors */

    SoftTimer errorLogTimer;
    SoftTimer_Create(&errorLogTimer, 2000000);
    while(1)
    {
        if (SoftTimer_Check(&errorLogTimer)) Logger_Log(origin, level, messageID, payload, payloadSize);
        UserInterface_Run();
        Receiver_Run();
        Logger_Flush();
    }
}

static void initializeLogger (void)
{
    HAL_UART_Init(115200);
    Logger_Create(LOGGER_MODE_MIXED);

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(UART_Transmit, TRANSMITTER_CALLBACK_GROUP_DELAYED);
    Transmitter_AttachTransmitCallback(ErrorLoop, TRANSMITTER_CALLBACK_GROUP_INSTANT);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_ERROR, true, true); /* Enable all error messages */
    Logger_SetFilter(LOG_SUBSYS_CURRENT, LOG_LEVEL_TRACE, true, true);
}


static void initializeReceiver (void)
{
    CommandHandler_Create(Commands_GetCommandTable());
    Receiver_Create(true, HAL_UART_ReceivePayload, micros);       
}
void setup (void)
{
    UserInterface_Create();
    UserInterface_AddButtonFunction(buttonFunction);

    initializeLogger();
    initializeReceiver();
    
    Magnetometer_Create();
    CurrentSensor_Create();

    SoftTimer_Create(&loopTimer, SYSTEM_LOOP_PERIOD_US);

    log_system_trace_initialized();
}

void loop (void)
{
    unsigned long looptime = micros();

    Magnetometer_NewRead();
    if (yawControllerRunning) YawController_Run();
    CurrentSensor_NewRead();
    UserInterface_Run();
    Receiver_Run();
    Logger_Flush();
    
    looptime = micros() - looptime;
    log_system_trace_loop_time(looptime);
    if (looptime > SYSTEM_LOOP_PERIOD_US)
    {
        log_system_error_loop_time_exceeded();
    }
    
    while(!SoftTimer_Check(&loopTimer));
}