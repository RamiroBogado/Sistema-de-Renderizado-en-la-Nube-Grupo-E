#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <mutex>
#include <queue>
#include "Job.h"
#include "Semaforo.h"

struct MessageQueue {
    std::queue<Job> colaPremium;
    std::queue<Job> colaFree;

    std::mutex mutexCola;
    Semaforo hayDatos;

    bool produccionFinalizada = false;
    int consumidoresPendientesCierre = 0;
    int premiumSeguidos = 0;

    static const int LIMITE_PREMIUM_SEGUIDOS = 5;

    MessageQueue();

    void insertar(Job job);
    bool extraer(Job& job);
    void finalizarProduccion(int cantidadConsumidores);
    bool estaVacia();
    void reiniciar();
};

#endif
