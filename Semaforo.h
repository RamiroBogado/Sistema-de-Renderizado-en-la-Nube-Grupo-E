#ifndef SEMAFORO_H
#define SEMAFORO_H

#include <condition_variable>
#include <mutex>

struct Semaforo {
    int contador;
    std::mutex mutexSemaforo;
    std::condition_variable condicion;

    Semaforo(int valorInicial = 0);

    void wait();
    void signal();
    void reiniciar(int nuevoValor);
};

#endif
