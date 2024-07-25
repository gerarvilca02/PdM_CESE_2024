
# Práctica 2

## Objetivo
Implementar un módulo de software para trabajar con retardos no bloqueantes.

## Punto 1
Implementar las funciones auxiliares necesarias para usar retardos no bloqueantes en un archivo fuente `main.c` con su correspondiente archivo de cabecera `main.h`.

### main.h
Se deben ubicar los prototipos de las siguientes funciones y las declaraciones:
```c
typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile?
typedef bool bool_t;     // Qué biblioteca se debe incluir para que esto compile?

typedef struct {
    tick_t startTime;
    tick_t duration;
    bool_t running;
} delay_t;

void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);
```

### main.c
Se debe implementar las siguientes funciones:

- `delayInit`: Carga el valor de duración del retardo en la estructura. No inicia el conteo del retardo y debe inicializar el flag `running` en `false`.
- `delayRead`: Verifica el estado del flag `running`:
  - Si es `false`, toma la marca de tiempo y cambia `running` a `true`.
  - Si es `true`, calcula si el tiempo del retardo se cumplió: `marca de tiempo actual - marca de tiempo inicial >= duración del retardo`.
  - Devuelve un valor booleano que indica si el tiempo se cumplió o no. Cuando el tiempo se cumple, cambia el flag `running` a `false`.
- `delayWrite`: Permite cambiar el tiempo de duración de un delay existente.

**Nota:** Para obtener una marca de tiempo se puede usar la función `HAL_GetTick()`, que devuelve un valor que se incrementa cada 1 ms y se puede usar como base de tiempo.

## Punto 2
Sobre el código desarrollado para el Punto 1 y sobre el mismo proyecto, implementar un programa que utilice retardos no bloqueantes y haga parpadear el LED de la placa de desarrollo: 100 ms encendido, 100 ms apagado en forma periódica.

## Punto 3 [Opcional]
Sobre el código desarrollado para el Punto 2 y sobre el mismo proyecto, implementar un programa que haga parpadear el LED de la placa de desarrollo en forma periódica con el siguiente patrón:
- 5 veces con período de 1 segundo y ciclo de trabajo del 50%.
- 5 veces con período de 200 ms y ciclo de trabajo del 50%.
- 5 veces con período de 100 ms y ciclo de trabajo del 50%.

Utilizar un vector o arreglo para definir el patrón y cambiar los tiempos de parpadeo.
