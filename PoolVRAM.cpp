#include "PoolVRAM.h"
#include <chrono>
#include <thread>

PoolVRAM::PoolVRAM()
    : slotsDisponibles(CAPACIDAD_MAXIMA) {
}

void PoolVRAM::asignar(Job& job) {
    slotsDisponibles.wait();

    // Serializa las asignaciones exitosas para cumplir el retardo de 450ms.
    std::lock_guard<std::mutex> lockAsignacion(mutexAsignacion);

    {
        std::lock_guard<std::mutex> lockVRAM(mutexVRAM);

        job.estado = ASIGNADO_VRAM;
        job.tiempoAsignadoVRAM = std::chrono::steady_clock::now();
        slots.push_back(job);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(450));
}

void PoolVRAM::liberar(const Job& job) {
    // Serializa las liberaciones para cumplir el retardo de 250ms.
    std::lock_guard<std::mutex> lockLiberacion(mutexLiberacion);

    bool liberado = false;

    {
        std::lock_guard<std::mutex> lockVRAM(mutexVRAM);

        for (auto it = slots.begin(); it != slots.end(); ++it) {
            if (it->id == job.id) {
                slots.erase(it);
                liberado = true;
                break;
            }
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    if (liberado) {
        slotsDisponibles.signal();
    }
}

int PoolVRAM::cantidadActiva() {
    std::lock_guard<std::mutex> lock(mutexVRAM);
    return static_cast<int>(slots.size());
}

void PoolVRAM::reiniciar() {
    std::lock_guard<std::mutex> lock(mutexVRAM);
    slots.clear();
    slotsDisponibles.reiniciar(CAPACIDAD_MAXIMA);
}
