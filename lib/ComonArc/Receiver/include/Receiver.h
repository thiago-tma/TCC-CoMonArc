#ifndef D_COMONARCRECEIVER_H
#define D_COMONARCRECEIVER_H

#define RECEIVER_MAX_COMMAND_BUFFER_SIZE 30
#define RECEIVER_TIMEOUT_MILLISECONDS    10
#define RECEIVER_TIMEOUT_MICROSECONDS    10000

#include <stdbool.h>
#include <stddef.h>

/* Receiver coleta uma stream de caracteres de uma fonte e identifica um comando com a terminação da stream por \n */
/* Caso a stream acumulada não recebe caracterres dentor de um prazo, a stream é descartada por um timeout */
/* Receber novos caracteres dentro do prazo e ainda não completar uma mensagem reinicia o período de timeout*/

typedef void (*Receiver_Callback_t)(char * message, size_t messageSize);

/* Receiver pode receber uma função de callback para chamar sempre que for executado ou fornecer uma função para recepção de comandos*/
void Receiver_Create (bool callbackOperation, Receiver_Callback_t callback);
void Receiver_Destroy (void);

/* Usar com operação de callback */
void Receiver_Run();

/* Outro componente recebe e repassa mensagem */
void Receiver_ReceiveMessage(char * message, size_t messageSize);

#endif /*D_COMONARCRECEIVER_H*/