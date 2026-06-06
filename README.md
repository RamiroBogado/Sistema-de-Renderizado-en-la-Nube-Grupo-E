# Sistema de Renderizado en la Nube - Grupo E

Trabajo práctico integrador de Programación Concurrente.

## Compilación sugerida

```bash
g++ -std=c++17 main.cpp Menu.cpp RenderFarm.cpp MessageQueue.cpp PoolVRAM.cpp Logger.cpp Semaforo.cpp -pthread -o renderfarm
```

## Archivos principales

- `Job.h`: estructura de los jobs y enums de prioridad/estado.
- `MessageQueue.*`: buffer de entrada con colas Premium/Free y anti-starvation.
- `PoolVRAM.*`: pool limitado a 5 slots con semáforo de capacidad.
- `RenderFarm.*`: coordinación de productores, consumidores y pruebas.
- `Logger.*`: logging sincronizado en `sistema.log`.
- `Semaforo.*`: TDA semáforo implementado con mutex y condition_variable.
- `Menu.*`: menú para ejecutar cada escenario bajo configuraciones A, B y C.

## Nota de entrega

Antes de entregar, completar en el informe el enlace real del repositorio GitHub público y el enlace al video demostrativo.
