#include "RenderFarm.h"
#include <chrono>
#include <iostream>
#include <thread>

RenderFarm::RenderFarm()
    : logger("sistema.log") {
}

Prioridad RenderFarm::generarPrioridad(int idJob) {
    if (modo == SOLO_PREMIUM) {
        return PREMIUM;
    }

    if (modo == ANTI_STARVATION) {
        if (idJob <= 50) {
            return PREMIUM;
        }

        if (idJob <= 60) {
            return FREE;
        }

        return PREMIUM;
    }

    if (idJob % 3 == 0) {
        return FREE;
    }

    return PREMIUM;
}

void RenderFarm::productor(int idProductor) {
    while (true) {
        int idJobActual;

        {
            std::lock_guard<std::mutex> lock(mutexCreacion);

            if (jobsCreados >= totalJobs) {
                break;
            }

            jobsCreados++;
            idJobActual = jobsCreados;
        }

        // En la prueba Anti-Starvation se simula un flujo exclusivo Premium
        // durante 5000ms. Luego ingresan Free y después más Premium.
        if (modo == ANTI_STARVATION && idJobActual > 50) {
            std::this_thread::sleep_until(
                inicioPrueba + std::chrono::milliseconds(5000)
            );
        }

        Job job;
        job.id = idJobActual;
        job.prioridad = generarPrioridad(idJobActual);
        job.estado = CREADO;

        logger.registrar(job, "CREADO");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        job.estado = EN_COLA;
        messageQueue.insertar(job);

        logger.registrar(job, "EN_COLA");

        {
            std::lock_guard<std::mutex> lock(mutexSalida);
            std::cout << "Productor " << idProductor
                      << " creo Job " << job.id
                      << " (" << prioridadToString(job.prioridad) << ")"
                      << std::endl;
        }
    }
}

void RenderFarm::consumidor(int idConsumidor) {
    while (true) {
        Job job;

        bool pudoExtraer = messageQueue.extraer(job);

        if (!pudoExtraer) {
            break;
        }

        poolVRAM.asignar(job);

        logger.registrar(job, "ASIGNADO_VRAM");

        {
            std::lock_guard<std::mutex> lock(mutexSalida);
            std::cout << "Consumidor " << idConsumidor
                      << " procesa Job " << job.id
                      << " (" << prioridadToString(job.prioridad) << ")"
                      << " | VRAM activa: " << poolVRAM.cantidadActiva()
                      << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(600));

        job.estado = FINALIZADO;

        poolVRAM.liberar(job);

        logger.registrar(job, "FINALIZADO");

        {
            std::lock_guard<std::mutex> lock(mutexFinalizados);
            jobsFinalizados++;
        }
    }
}

void RenderFarm::ejecutar(int cantidadProductores, int cantidadConsumidores, int cantidadJobs, ModoPrueba modoPrueba) {
    totalJobs = cantidadJobs;
    jobsCreados = 0;
    jobsFinalizados = 0;
    modo = modoPrueba;
    inicioPrueba = std::chrono::steady_clock::now();

    messageQueue.reiniciar();
    poolVRAM.reiniciar();

    std::vector<std::thread> productores;
    std::vector<std::thread> consumidores;

    std::cout << "======================================" << std::endl;
    std::cout << "Iniciando sistema de renderizado" << std::endl;
    std::cout << "Productores: " << cantidadProductores << std::endl;
    std::cout << "Consumidores: " << cantidadConsumidores << std::endl;
    std::cout << "Total de jobs: " << totalJobs << std::endl;
    std::cout << "======================================" << std::endl;

    for (int i = 1; i <= cantidadConsumidores; i++) {
        consumidores.push_back(std::thread(&RenderFarm::consumidor, this, i));
    }

    for (int i = 1; i <= cantidadProductores; i++) {
        productores.push_back(std::thread(&RenderFarm::productor, this, i));
    }

    for (auto& productor : productores) {
        productor.join();
    }

    messageQueue.finalizarProduccion(cantidadConsumidores);

    for (auto& consumidor : consumidores) {
        consumidor.join();
    }

    std::cout << "======================================" << std::endl;
    std::cout << "Sistema finalizado" << std::endl;
    std::cout << "Jobs creados: " << jobsCreados << std::endl;
    std::cout << "Jobs finalizados: " << jobsFinalizados << std::endl;
    std::cout << "======================================" << std::endl;
}
