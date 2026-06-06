#ifndef RENDERFARM_H
#define RENDERFARM_H

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.h"
#include "MessageQueue.h"
#include "PoolVRAM.h"

enum ModoPrueba {
    NORMAL,
    SOLO_PREMIUM,
    ANTI_STARVATION
};

struct RenderFarm {
    MessageQueue messageQueue;
    PoolVRAM poolVRAM;
    Logger logger;

    int totalJobs = 0;
    int jobsCreados = 0;
    int jobsFinalizados = 0;

    ModoPrueba modo = NORMAL;

    std::chrono::steady_clock::time_point inicioPrueba;

    std::mutex mutexCreacion;
    std::mutex mutexFinalizados;
    std::mutex mutexSalida;

    RenderFarm();

    void ejecutar(int cantidadProductores, int cantidadConsumidores, int cantidadJobs, ModoPrueba modoPrueba);

    void productor(int idProductor);
    void consumidor(int idConsumidor);

    Prioridad generarPrioridad(int idJob);
};

#endif
