# Práctica 4: Anti-Rebote (Debounce)

## Objetivo
Implementar una Máquina de Estados Finitos (MEF) para manejar el anti-rebote por software.

## Tareas Principales

### 1. Crear Proyecto
- Basado en el proyecto de la práctica 3.

### 2. MEF Anti-Rebote
- **Estados:**
  - `BUTTON_UP`
  - `BUTTON_FALLING`
  - `BUTTON_DOWN`
  - `BUTTON_RAISING`
- **Estado Inicial:**
  - `BUTTON_UP`

### 3. Funciones en `main.c`
- `void debounceFSM_init();` – Inicializa el estado.
- `void debounceFSM_update();` – Lee entradas y actualiza estados.
- `void buttonPressed();` – Enciende el LED.
- `void buttonReleased();` – Apaga el LED.

### 4. Anti-Rebote
- **Tiempo:** 40 ms con retardo no bloqueante.
- `debounceFSM_update()` debe llamarse periódicamente.

### 5. Módulo de Software (`API_debounce.c` y `API_debounce.h`)
- **Prototipos:**
  - `void debounceFSM_init();`
  - `void debounceFSM_update();`
  - `bool_t readKey();`
- `readKey()` devuelve true si se presiona la tecla y resetea el estado.

### 6. Programa
- Cambia la frecuencia de parpadeo del LED entre 100 ms y 500 ms al presionar la tecla.
- Usa `API_debounce` y retardos no bloqueantes de `API_delay`.

