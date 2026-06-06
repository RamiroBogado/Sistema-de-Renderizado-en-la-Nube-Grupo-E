# Sistema de Renderizado en la Nube - Grupo E

Trabajo Práctico Integrador de **Programación Concurrente** desarrollado en **C++**.  
El proyecto simula una granja de renderizado (Cloud Render Farm) utilizando múltiples hilos, sincronización, exclusión mutua y control de recursos compartidos.

---

# Objetivo

Implementar un sistema concurrente capaz de gestionar tareas de renderizado pesado (*Jobs*) mediante:

- Productores (API Gateway)
- Consumidores (Worker Nodes)
- Cola de mensajes (Message Queue)
- Pool de VRAM limitado
- Priorización de tareas
- Prevención de starvation
- Logging sincronizado

El sistema fue desarrollado aplicando conceptos de:

- Threads
- Mutex
- Semáforos
- Condition Variables
- Exclusión mutua
- Productor-Consumidor
- Race Conditions
- Starvation
- Logging concurrente

---

# Arquitectura del Sistema

El sistema está compuesto por los siguientes módulos:

## `MessageQueue`

Gestiona la cola de trabajos pendientes utilizando:

- Cola Premium
- Cola Free
- Prioridad de despacho
- Anti-starvation

Características:

- Los jobs Premium tienen prioridad.
- Luego de cierta cantidad de Premium consecutivos, se fuerza el procesamiento de un Free.
- Uso de mutex + semáforo.

---

## `PoolVRAM`

Simula la memoria de video compartida.

Características:

- Máximo de 5 jobs simultáneos.
- Exclusión mutua en asignación/liberación.
- Retardos simulados:
  - 450ms asignación
  - 250ms liberación

---

## `RenderFarm`

Coordina todo el sistema:

- Creación de productores
- Creación de consumidores
- Generación de jobs
- Ejecución de pruebas
- Control de concurrencia

También implementa:

- Prueba Anti-Starvation
- Prueba de saturación
- Prueba de carga masiva
- Contador global protegido con mutex

---

## `Logger`

Registra eventos del sistema en `sistema.log`.

Formato:

```txt
[Timestamp] - Job ID - Prioridad - Evento
```

Eventos registrados:

- CREADO
- EN_COLA
- ASIGNADO_VRAM
- FINALIZADO

---

## `Semaforo`

TDA Semáforo implementado manualmente utilizando:

- `mutex`
- `condition_variable`

Operaciones:

- `wait()`
- `signal()`
- `reiniciar()`

---

# Características Concurrentes Implementadas

## Productor - Consumidor

Sincronización entre:

- Productores → generan jobs
- Consumidores → procesan jobs

---

## Exclusión Mutua

Uso de `mutex` para proteger:

- Message Queue
- Pool VRAM
- Logger
- Contadores globales

---

## Semáforos

Controlan:

- Disponibilidad de jobs
- Capacidad de VRAM

---

## Anti-Starvation

Los jobs Free no quedan bloqueados indefinidamente.

---

# Integrantes

- Ramiro Enzo Bogado Leon
- Martín Ezequiel Barreto
- Nicolas Ariel Basalo
- Sebastian Oliveto

---

# Tecnologías Utilizadas

- C++
- STL Threads
- Mutex
- Condition Variable
- Semáforos
- Programación Concurrente

---

# Requisitos Académicos Cubiertos

✔ Programación Concurrente  
✔ Productor-Consumidor  
✔ Exclusión Mutua  
✔ Race Conditions  
✔ Starvation  
✔ Logging sincronizado  
✔ Pool de recursos compartidos  
✔ Sincronización de hilos  
✔ Manejo de prioridades  

---
