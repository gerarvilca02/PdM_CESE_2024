#include "main.h"
#include "API_uart.h"
#include "API_ADC.h"
#include "GPIO.h"
#include "Timer.h"
#include <stdio.h>

// Definición de los estados del sistema
typedef enum {
    ESTADO_INICIO,
    ESTADO_MONITOREO,
    ESTADO_RIEGO,
    ESTADO_PAUSA,
    ESTADO_ALERTA_BAJA,
    ESTADO_ALERTA_CRITICA
} estado_t;

// Variables globales
estado_t estadoActual = ESTADO_INICIO;
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
char message[100];  // Buffer para los mensajes
uint32_t adcValue;
uint8_t percentage;
delay_t delayPausa;
delay_t delayRiego;
uint32_t tiempoRiegoInicio;
uint32_t tiempoRiegoSegundos;
uint32_t tiempoRestanteRiego;  // Variable para almacenar el tiempo restante
uint32_t tiempoGuardadoRiego;  // Variable para almacenar el tiempo restante en caso de alerta crítica

void iniciarSistema() {
    HAL_Init();

    ADC_Init();
    uartInit();
    GPIO_Init();
    delayInit(&delayRiego, 1000); // Riego controlado por delay de 1 segundo

    // Mover el cursor a la parte superior izquierda
    sprintf(message, "\033[H");
    uartSendString((uint8_t*)message);

    // Mensajes iniciales estáticos
    sprintf(message, "Sensor de Humedad: Húmedo   \r\n");
    uartSendString((uint8_t*)message);

    sprintf(message, "Sensor PIR: Sin presencia   \r\n");
    uartSendString((uint8_t*)message);

    sprintf(message, "Sensor LDR: Día   \r\n");
    uartSendString((uint8_t*)message);

    sprintf(message, "Capacidad del tanque: 0%%, Presión: 0 Pa\r\n");
    uartSendString((uint8_t*)message);
}

void manejarEstadoInicio() {
    iniciarSistema();
    estadoActual = ESTADO_MONITOREO;
}

void manejarEstadoRiego() {
    // Verificar si se detecta una persona o si el nivel de agua es crítico
    uint8_t estadoPIR = Leer_Pulsador_PIR();
    if (estadoPIR == 1) {
        estadoActual = ESTADO_PAUSA;
        tiempoRestanteRiego = tiempoRiegoSegundos;  // Guardar el tiempo restante
        LED_Bomba_Off();  // Apagar la bomba durante la pausa
        return;
    } else if (percentage <= 10) {
        estadoActual = ESTADO_ALERTA_CRITICA;
        tiempoGuardadoRiego = tiempoRiegoSegundos;  // Guardar el tiempo restante en caso de alerta crítica
        LED_Bomba_Off();  // Se apaga la bomba de agua para prevenir daños si el nivel de agua cae por debajo del 10%.
        return;
    } else if (percentage <= 20 && percentage > 10) {
        // Estado de alerta baja, no se detiene el riego, pero se muestra el mensaje
        sprintf(message, "\033[6HAlerta: Nivel bajo de agua! \r\n");
        uartSendString((uint8_t*)message);
    } else {
    	sprintf(message, "\033[6HTanque con nivel de agua óptimo \r\n");
		uartSendString((uint8_t*)message);
    }

    // Continuar la cuenta regresiva si no está en pausa y no hay condiciones críticas
    if (delayRead(&delayRiego)) {
        if (tiempoRiegoSegundos > 0) {
            tiempoRiegoSegundos--;  // Decrementar el contador de tiempo de riego
            // Actualizar la cuenta regresiva en la terminal
            sprintf(message, "\033[5HRegando... Tiempo restante: %d segundos   ", tiempoRiegoSegundos);
            uartSendString((uint8_t*)message);

            // Reiniciar el temporizador para el próximo segundo
            delayWrite(&delayRiego, 1000);
        } else {
            // Si el tiempo de riego ha terminado
            LED_Bomba_Off();  // Apagar la bomba después de finalizar el riego
            estadoActual = ESTADO_MONITOREO;  // Volver al monitoreo después del riego
        }
    }

    // Leer y actualizar estados de los sensores mientras se riega
    manejarEstadoMonitoreo();  // Solo actualizar mensajes, no interferir con la lógica del riego
}

void manejarEstadoMonitoreo() {
    // Leer y actualizar estados de los sensores
    uint8_t estadoHumedad = Leer_Pulsador_Humedad();
    uint8_t estadoPIR = Leer_Pulsador_PIR();
    uint8_t estadoLDR = Leer_Pulsador_LDR();

    // Actualizar los mensajes según los valores actuales
    sprintf(message, "\033[H\033[20C%s", estadoHumedad ? "Húmedo   " : "Seco     ");
    uartSendString((uint8_t*)message);

    sprintf(message, "\033[2H\033[20C%s", estadoPIR ? "Persona detectada!" : "Sin presencia      ");
    uartSendString((uint8_t*)message);

    sprintf(message, "\033[3H\033[20C%s", estadoLDR ? "Día      " : "Noche    ");
    uartSendString((uint8_t*)message);

    // Leer ADC para el nivel del tanque
    adcValue = ADC_Read();
    percentage = (adcValue * 100) / 4095;
    int altura_cm = percentage * 3;  // Simulación de altura en cm
    int presion_Pa = altura_cm * 98;

    // Actualizar la capacidad del tanque y la presión
    sprintf(message, "\033[4H\033[21C%3d%%, Presión: %4d Pa", percentage, presion_Pa);
    uartSendString((uint8_t*)message);

    // Verificar transiciones, pero sin interrumpir el estado de riego
    if (estadoActual != ESTADO_RIEGO) {
        if (estadoHumedad == 0) {
            estadoActual = ESTADO_RIEGO;  // Si el suelo está seco, pasar al estado de riego
            tiempoRiegoSegundos = estadoLDR ? 30 : 15;  // Reiniciar tiempo de riego según sea día o noche
            tiempoRestanteRiego = 0;
            tiempoGuardadoRiego = 0;
            tiempoRiegoInicio = HAL_GetTick();  // Guardar el tiempo de inicio del riego
            LED_Bomba_On();
        } else if (percentage <= 20 && percentage > 10) {
            estadoActual = ESTADO_ALERTA_BAJA;
        } else if (percentage <= 10) {
            estadoActual = ESTADO_ALERTA_CRITICA;
        }
    }
}

void manejarEstadoPausa() {
    // Actualizar el mensaje indicando que el sistema está en pausa por detección de persona
    sprintf(message, "\033[5HPausa por detección de persona...\r\n");
    uartSendString((uint8_t*)message);

    // Leer el estado del PIR para ver si la persona ya se ha ido
    uint8_t estadoPIR = Leer_Pulsador_PIR();
    if (estadoPIR == 0) {  // Si la persona ya no está presente
        estadoActual = ESTADO_RIEGO;
        // Continuar desde el tiempo restante guardado
        tiempoRiegoSegundos = tiempoRestanteRiego;
        LED_Bomba_On();  // Reanudar la bomba
        delayWrite(&delayRiego, 1000);  // Continuar el temporizador de cuenta regresiva
    }

    // Continuar actualizando los valores de los sensores en la terminal durante la pausa
    manejarEstadoMonitoreo();
}

void manejarEstadoAlertaCritica() {
    sprintf(message, "\033[6HAlerta: Nivel crítico de agua!\r\n");
    uartSendString((uint8_t*)message);
    LED_Alerta_On();  // Encender el LED de alerta

    // Permitir la actualización del monitoreo durante la alerta
    manejarEstadoMonitoreo();

    // Reanudar riego si el nivel sube por encima del 10%
    if (percentage > 10) {
        LED_Alerta_Off();
        estadoActual = ESTADO_RIEGO;
        // Continuar desde el tiempo guardado
        tiempoRiegoSegundos = tiempoGuardadoRiego;
        LED_Bomba_On();  // Encender la bomba nuevamente
        delayWrite(&delayRiego, 1000);  // Continuar el temporizador de cuenta regresiva
    }
}

void manejarEstadoAlertaBaja() {
    sprintf(message, "\033[6HAlerta: Nivel bajo de agua!\r\n");
    uartSendString((uint8_t*)message);
    LED_Alerta_On();  // Encender el LED de alerta

    // Permitir la actualización del monitoreo durante la alerta
    manejarEstadoMonitoreo();

    // Verificar si el nivel baja a crítico o vuelve a un nivel seguro
    if (percentage > 20) {
    	sprintf(message, "\033[6HTanque con nivel óptimo \r\n");
    	uartSendString((uint8_t*)message);
        estadoActual = ESTADO_RIEGO;  // Continuar el riego
    } else if (percentage <= 10) {
        tiempoGuardadoRiego = tiempoRiegoSegundos;  // Guardar el tiempo restante en caso de alerta crítica
        estadoActual = ESTADO_ALERTA_CRITICA;
    }
}

void loop() {
    switch (estadoActual) {
        case ESTADO_INICIO:
            manejarEstadoInicio();
            break;
        case ESTADO_MONITOREO:
            manejarEstadoMonitoreo();
            break;
        case ESTADO_RIEGO:
            manejarEstadoRiego();
            break;
        case ESTADO_PAUSA:
            manejarEstadoPausa();
            break;
        case ESTADO_ALERTA_BAJA:
            manejarEstadoAlertaBaja();
            break;
        case ESTADO_ALERTA_CRITICA:
            manejarEstadoAlertaCritica();
            break;
    }

    HAL_Delay(100);
}

int main(void) {
    while (1) {
        loop();
    }
}

void Error_Handler(void) {
    while (1) {

    }
}
