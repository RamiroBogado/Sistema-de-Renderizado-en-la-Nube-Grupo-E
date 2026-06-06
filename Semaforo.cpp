#include "Semaforo.h"

Semaforo::Semaforo(int valorInicial) {
    contador = valorInicial;
}

void Semaforo::wait() {
    std::unique_lock<std::mutex> lock(mutexSemaforo);

    while (contador == 0) {
        condicion.wait(lock);
    }

    contador--;
}

void Semaforo::signal() {
    std::unique_lock<std::mutex> lock(mutexSemaforo);

    contador++;
    condicion.notify_one();
}

void Semaforo::reiniciar(int nuevoValor) {
    std::unique_lock<std::mutex> lock(mutexSemaforo);

    contador = nuevoValor;
    condicion.notify_all();
}
