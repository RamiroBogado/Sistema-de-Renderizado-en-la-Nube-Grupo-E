#include "MessageQueue.h"

MessageQueue::MessageQueue()
    : hayDatos(0) {
}

void MessageQueue::insertar(Job job) {
    {
        std::lock_guard<std::mutex> lock(mutexCola);

        if (job.prioridad == PREMIUM) {
            colaPremium.push(job);
        } else {
            colaFree.push(job);
        }
    }

    hayDatos.signal();
}

bool MessageQueue::extraer(Job& job) {
    hayDatos.wait();

    std::lock_guard<std::mutex> lock(mutexCola);

    // Anti-starvation: si ya salieron demasiados Premium seguidos
    // y hay trabajos Free esperando, se fuerza un Free.
    if (!colaFree.empty() && premiumSeguidos >= LIMITE_PREMIUM_SEGUIDOS) {
        job = colaFree.front();
        colaFree.pop();
        premiumSeguidos = 0;
        return true;
    }

    // Prioridad normal: Premium antes que Free.
    if (!colaPremium.empty()) {
        job = colaPremium.front();
        colaPremium.pop();
        premiumSeguidos++;
        return true;
    }

    if (!colaFree.empty()) {
        job = colaFree.front();
        colaFree.pop();
        premiumSeguidos = 0;
        return true;
    }

    // Señal de cierre: se usa una cola lógica de cierres separada
    // de las colas Premium/Free para no cortar consumidores antes de tiempo.
    if (produccionFinalizada && consumidoresPendientesCierre > 0) {
        consumidoresPendientesCierre--;
        return false;
    }

    return false;
}

void MessageQueue::finalizarProduccion(int cantidadConsumidores) {
    {
        std::lock_guard<std::mutex> lock(mutexCola);
        produccionFinalizada = true;
        consumidoresPendientesCierre += cantidadConsumidores;
    }

    for (int i = 0; i < cantidadConsumidores; i++) {
        hayDatos.signal();
    }
}

bool MessageQueue::estaVacia() {
    std::lock_guard<std::mutex> lock(mutexCola);
    return colaPremium.empty() && colaFree.empty();
}

void MessageQueue::reiniciar() {
    std::lock_guard<std::mutex> lock(mutexCola);

    while (!colaPremium.empty()) {
        colaPremium.pop();
    }

    while (!colaFree.empty()) {
        colaFree.pop();
    }

    produccionFinalizada = false;
    consumidoresPendientesCierre = 0;
    premiumSeguidos = 0;

    hayDatos.reiniciar(0);
}
