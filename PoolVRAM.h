#ifndef POOLVRAM_H
#define POOLVRAM_H

#include <mutex>
#include <vector>
#include "Job.h"
#include "Semaforo.h"

struct PoolVRAM {
    std::vector<Job> slots;

    std::mutex mutexVRAM;
    std::mutex mutexAsignacion;
    std::mutex mutexLiberacion;
    Semaforo slotsDisponibles;

    static const int CAPACIDAD_MAXIMA = 5;

    PoolVRAM();

    void asignar(Job& job);
    void liberar(const Job& job);
    int cantidadActiva();
    void reiniciar();
};

#endif
