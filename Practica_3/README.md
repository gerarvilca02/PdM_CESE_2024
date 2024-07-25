# Práctica 3 - Modularización

## Objetivo
Implementar un módulo de software para trabajar con retardos no bloqueantes a partir de las funciones creadas en la práctica 2.

## Instrucciones

### Punto 1
1. Crear un nuevo proyecto como copia del proyecto realizado para la práctica 2.
2. Crear una carpeta `API` dentro de la carpeta `Drivers` en la estructura de directorios del nuevo proyecto.
   - Dentro de la carpeta `API`, crear subcarpetas `/Src` y `/Inc`.
3. Encapsular las funciones necesarias para usar retardos no bloqueantes en un archivo fuente `API_delay.c` con su correspondiente archivo de cabecera `API_delay.h`.
   - En `API_delay.h` se deben ubicar los prototipos de las funciones y declaraciones:
     ```c
     typedef uint32_t tick_t;
     typedef bool bool_t;

     typedef struct {
        tick_t startTime;
        tick_t duration;
        bool_t running;
     } delay_t;

     void delayInit( delay_t * delay, tick_t duration );
     bool_t delayRead( delay_t * delay );
     void delayWrite( delay_t * delay, tick_t duration );
     ```
   - En `API_delay.c` se deben ubicar la implementación de todas las funciones.
   - Nota: Cuando se agregan carpetas a un proyecto de Eclipse, se deben incluir en el include path para que se incluya su contenido en la compilación. Se debe hacer clic derecho sobre la carpeta con los archivos de encabezado y seleccionar la opción `add/remove include path`.

### Punto 2
1. Implementar un programa que utilice retardos no bloqueantes y haga titilar en forma periódica un LED de la placa NUCLEO-F4xx de acuerdo a una secuencia predeterminada:
   ```c
   const uint32_t TIEMPOS[] = {500, 100, 100, 1000};
   ```
2. Utilizar la función `delayWrite` y una única variable tipo `delay_t` para cambiar el tiempo de encendido del LED.
   - Nota: Los tiempos indicados son de encendido y el LED debe trabajar con un duty cycle del 50%.

### Punto 3
1. Implementar la siguiente función auxiliar pública en `API_delay.c`:
   ```c
   bool_t delayIsRunning(delay_t * delay);
   ```
   - Esta función debe devolver una copia del valor del campo `running` de la estructura `delay_t`.
2. Utilizar esta función en el código implementado para el punto 2 para verificar que el delay no esté corriendo antes de cambiar su valor con `delayWrite`.
