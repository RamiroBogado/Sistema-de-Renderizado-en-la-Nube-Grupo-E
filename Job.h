#ifndef JOB_H
#define JOB_H

#include <chrono>
#include <string>

enum Prioridad {
    FREE = 0,
    PREMIUM = 1
};

enum EstadoJob {
    CREADO,
    EN_COLA,
    ASIGNADO_VRAM,
    FINALIZADO
};

struct Job {
    int id;
    Prioridad prioridad;
    EstadoJob estado;
    std::chrono::steady_clock::time_point tiempoAsignadoVRAM;
};

inline std::string prioridadToString(Prioridad prioridad) {
    return prioridad == PREMIUM ? "Premium" : "Free";
}

#endif
