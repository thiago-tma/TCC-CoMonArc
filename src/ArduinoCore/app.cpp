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
    HAL_UART_Init(19200);
    Logger_Create(LOGGER_MODE_MIXED);

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(UART_Transmit, TRANSMITTER_CALLBACK_GROUP_DELAYED);
    Transmitter_AttachTransmitCallback(ErrorLoop, TRANSMITTER_CALLBACK_GROUP_INSTANT);

    /* Configuração de cenários cumulativo (descomentar do cenário 1 ao X para executar cenário X)*/
    /* Cenário 1 */
    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_EVENT, true, true); 

    /* Cenário 2 */
    Logger_SetFilter(LOG_SUBSYS_MAGNETOMETER, LOG_LEVEL_DATA, true, true);
    Logger_SetFilter(LOG_SUBSYS_SERVO, LOG_LEVEL_DATA, true, true);

    /* Cenário 3 */
    Logger_SetFilter(LOG_SUBSYS_USER_INTERFACE, LOG_LEVEL_DATA, true, true);
    Logger_SetFilter(LOG_SUBSYS_CURRENT, LOG_LEVEL_DATA, true, true);

    /* Cenário 4 */
    Logger_ResetFilter();
    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_EVENT, true, true);
    Logger_SetFilter(LOG_SUBSYS_SYSTEM,LOG_LEVEL_TRACE, true, true);

    /* Cenário 5 */
    Logger_SetFilter(LOG_SUBSYS_MAGNETOMETER, LOG_LEVEL_DATA, true, true);
    Logger_SetFilter(LOG_SUBSYS_SERVO, LOG_LEVEL_DATA, true, true);
    Logger_SetFilter(LOG_SUBSYS_USER_INTERFACE, LOG_LEVEL_DATA, true, true);
    Logger_SetFilter(LOG_SUBSYS_CURRENT, LOG_LEVEL_DATA, true, true);

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
    unsigned long taskTime;

    /* Magnetometer task */
    log_system_trace_magnetometer_running();
    taskTime = micros();
    Magnetometer_NewRead();
    log_system_trace_magnetometer_time((micros()-taskTime));

    /* Yaw Controller task */
    if (yawControllerRunning)
    {
        log_system_trace_yaw_controller_running();
        taskTime = micros();
        YawController_Run();
        log_system_trace_yaw_controller_time((micros()-taskTime));
    } 

    /* Current Sensor task */
    log_system_trace_current_sensor_running();
    taskTime = micros();
    CurrentSensor_NewRead();
    log_system_trace_current_sensor_time((micros()-taskTime));

    /* UserInterface task */
    log_system_trace_user_interface_running();
    taskTime = micros();
    UserInterface_Run();
    log_system_trace_user_interface_time((micros()-taskTime));

    /* Command Handler task */
    log_system_trace_command_handler_running();
    taskTime = micros();
    Receiver_Run();
    log_system_trace_command_handler_time((micros()-taskTime));

    /* Logger task */
    log_system_trace_logger_running();
    taskTime = micros();
    Logger_Flush();
    log_system_trace_logger_time((micros()-taskTime));
    
    looptime = micros() - looptime;
    log_system_trace_loop_time(looptime);
    if (looptime > SYSTEM_LOOP_PERIOD_US)
    {
        log_system_error_loop_time_exceeded();
    }
    
    while(!SoftTimer_Check(&loopTimer));
}