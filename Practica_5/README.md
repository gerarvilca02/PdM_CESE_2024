# Práctica 5

## Objetivo

Implementar un módulo de software con un envoltorio de la HAL para trabajar con la UART.

## Punto 1

Crear un módulo de software en el archivo `API_uart.c` con su correspondiente archivo de cabecera `API_uart.h`, ubicándolos en las carpetas `/drivers/API/src` y `/drivers/API/inc` respectivamente.

### API_uart.h

Colocar los prototipos de las funciones públicas:

- `bool_t uartInit();`
- `void uartSendString(uint8_t * pstring);`
- `void uartSendStringSize(uint8_t * pstring, uint16_t size);`
- `void uartReceiveStringSize(uint8_t * pstring, uint16_t size);`

### API_uart.c

Implementar las funciones privadas y públicas del módulo.

### Consideraciones para la implementación

- `uartInit()` debe inicializar la UART e imprimir por la terminal serie un mensaje con sus parámetros de configuración. La función devuelve `True` si la inicialización es exitosa y `False` en caso contrario.
- `uartSendString(uint8_t *pstring)` envía un string completo por la UART utilizando `HAL_UART_Transmit(...)`.
- `uartSendStringSize(uint8_t * pstring, uint16_t size)` envía un string por la UART con una cantidad específica de caracteres utilizando `HAL_UART_Transmit(...)`.
- Las funciones deben verificar que todos los parámetros sean válidos: punteros no nulos y tamaños razonables.
- Verificar los valores de retorno de todas las funciones del módulo UART de la HAL utilizadas.

## Punto 2

Implementar un programa basado en la práctica 4 que use el módulo de UART para informar al usuario sobre flancos ascendentes y descendentes.

- Enviar un mensaje por la UART indicando "Flanco ascendente detectado" o "Flanco descendente detectado" en cada flanco.
- Escribir cada mensaje en una línea nueva en la terminal de la PC.
