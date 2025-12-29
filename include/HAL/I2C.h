#ifndef D_I2C_H
#define D_I2C_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    I2C_SPEED_STANDARD,
    I2C_SPEED_FAST,
    I2C_SPEED_FAST_PLUS
} I2C_Speed_Mode_t;

typedef enum
{
    I2C_MODE_MASTER,
    I2C_MODE_SLAVE
}   I2C_Operation_Mode_t;

typedef struct
{
    uint8_t channel;
    I2C_Operation_Mode_t mode;
    I2C_Speed_Mode_t speed;
} I2C_Handle_t;


void I2C_Create(I2C_Handle_t    * handle);
void I2C_Destroy(I2C_Handle_t   * handle);

/* Inicia com um sinal de start, o endereço e um bit de escrita, seguida pela escrita de N bytes */
/* 'stop' indica se último byte de escrita é acompanhado de um sinal de stop ou não */
void I2C_Master_Transmit(I2C_Handle_t * handle, uint8_t devAddr, uint8_t * data, uint16_t size, bool stop);

void I2C_Master_Receive(I2C_Handle_t *  handle, uint8_t devAddr, uint8_t *data, uint16_t size, bool stop);
#endif /* D_I2C_H */