#ifndef D_COMONARCRECEIVER_H
#define D_COMONARCRECEIVER_H

/* Receiver coleta uma stream de caracteres de uma fonte e identifica um comando com a terminação da stream por \n */
/* Caso a stream acumulada não recebe caracterres dentor de um prazo, a stream é descartada por um timeout */
/* Receber novos caracteres dentro do prazo e ainda não completar uma mensagem reinicia o período de timeout*/

#define RECEIVER_MAX_COMMAND_BUFFER_SIZE 30
#define RECEIVER_TIMEOUT_MILLISECONDS    10
#define RECEIVER_TIMEOUT_MICROSECONDS    10000
#define RECEIVER_COMMAND_TERMINATING_CHARACTER '\n'

#include <stdbool.h>
#include <stddef.h>

typedef unsigned long timeMicroseconds;

typedef enum
{
    RECEIVER_OK = 0,
    RECEIVER_ERROR_NOT_INITIALIZED,
    RECEIVER_ERROR_NO_COMMAND_CALLBACK,
    RECEIVER_ERROR_NO_TIMER_CALLBACK,
    RECEIVER_ERROR_BUFFER_CAPACITY_REACHED,
    RECEIVER_ERROR_COMMANDHANDLER_NOT_INITIALIZED,
    RECEIVER_ERROR_NO_COMMAND_FOUND,
    RECEIVER_ERROR_COMMAND_HANDLER_ERROR
} Receiver_Error_t;

/* Callback para recepção de dados */
/* messageMaxSize limita o número de bytes recebidos por vez */
/* message recebe cada byte */
/* messageSize deve conter o número de bytes inscritos */
typedef void (*Receiver_Command_Callback_t)(size_t messageMaxSize, char * message, size_t * messageSize);

/* Callback para implementação do timeout */
/* Retorna o tempo de sistema em microsegundos */
typedef timeMicroseconds (*Receiver_Timer_Callback_t)(void);

/* Receiver pode receber uma função de callback para chamar sempre que for executado ou fornecer uma função para recepção de comandos*/
Receiver_Error_t Receiver_Create (bool callbackOperation, Receiver_Command_Callback_t commandCallback, Receiver_Timer_Callback_t timerCallback);
Receiver_Error_t Receiver_Destroy (void);

/* Chamar periódicamente para manutenção do módulo */
/* Com operação de callback e um callback pelo qual coletar dados, essa é a única função necessária para a execução periódica do módulo*/
Receiver_Error_t Receiver_Run();

/* Outro componente recebe e repassa mensagem */
Receiver_Error_t Receiver_ReceiveMessage(const char * message, size_t messageSize, size_t * receivedBytes);

#endif /*D_COMONARCRECEIVER_H*/