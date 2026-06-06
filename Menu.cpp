#include "Menu.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#include "RenderFarm.h"

struct Configuracion {
    std::string nombre;
    int productores;
    int consumidores;
};

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    std::cout << "\n";
#ifdef _WIN32
    system("pause");
#else
    std::cout << "Presione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
#endif
}

int leerEntero() {
    int opcion;

    std::cin >> opcion;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        return -1;
    }

    return opcion;
}

void mostrarMenuPrincipal() {
    limpiarPantalla();

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "        SISTEMA DE RENDERIZADO EN LA NUBE - CLOUD FARM       \n";
    std::cout << "============================================================\n";
    std::cout << "  Seleccione una configuracion de ejecucion\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "  1) Configuracion A | 1 Productor  - 2 Consumidores\n";
    std::cout << "  2) Configuracion B | 3 Productores - 1 Consumidor\n";
    std::cout << "  3) Configuracion C | 3 Productores - 3 Consumidores\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "  0) Salir\n";
    std::cout << "============================================================\n";
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuEscenarios(const Configuracion& config) {
    limpiarPantalla();

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << " CONFIGURACION SELECCIONADA: " << config.nombre << "\n";
    std::cout << " Productores: " << config.productores
              << " | Consumidores: " << config.consumidores << "\n";
    std::cout << "============================================================\n";
    std::cout << "  1) Ejecucion normal       | 20 Jobs mixtos\n";
    std::cout << "  2) Prueba de vacuidad     | 0 Jobs\n";
    std::cout << "  3) Prueba de saturacion   | 8 Jobs Premium\n";
    std::cout << "  4) Prueba de carga masiva | 1500 Jobs mixtos\n";
    std::cout << "  5) Prueba Anti-Starvation | Premium + Free + Premium\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "  0) Volver\n";
    std::cout << "============================================================\n";
    std::cout << "Seleccione un escenario: ";
}

Configuracion obtenerConfiguracion(int opcion) {
    switch (opcion) {
        case 1:
            return {"A | 1 Productor - 2 Consumidores", 1, 2};

        case 2:
            return {"B | 3 Productores - 1 Consumidor", 3, 1};

        case 3:
            return {"C | 3 Productores - 3 Consumidores", 3, 3};

        default:
            return {"Invalida", 0, 0};
    }
}

void ejecutarEscenario(const Configuracion& config, int escenario) {
    if (escenario == 0) {
        return;
    }

    RenderFarm sistema;

    limpiarPantalla();

    int productores = config.productores;
    int consumidores = config.consumidores;
    int jobs = 20;
    ModoPrueba modo = NORMAL;
    std::string nombreEscenario;

    switch (escenario) {
        case 1:
            nombreEscenario = "EJECUCION NORMAL";
            jobs = 20;
            modo = NORMAL;
            break;

        case 2:
            nombreEscenario = "PRUEBA DE VACUIDAD";
            jobs = 0;
            modo = NORMAL;
            break;

        case 3:
            nombreEscenario = "PRUEBA DE SATURACION";
            jobs = 8;
            modo = SOLO_PREMIUM;

            // Para probar de verdad el limite de 5, debe haber mas consumidores
            // intentando asignar al mismo tiempo que slots disponibles.
            if (consumidores < 8) {
                consumidores = 8;
            }

            break;

        case 4:
            nombreEscenario = "PRUEBA DE CARGA MASIVA";
            jobs = 1500;
            modo = NORMAL;
            break;

        case 5:
            nombreEscenario = "PRUEBA ANTI-STARVATION";
            jobs = 80;
            modo = ANTI_STARVATION;
            break;

        default:
            std::cout << "Escenario invalido.\n";
            pausar();
            return;
    }

    std::cout << "============================================================\n";
    std::cout << " " << nombreEscenario << "\n";
    std::cout << " " << config.nombre << "\n";
    std::cout << "============================================================\n";

    if (escenario == 3) {
        std::cout << " Nota: para probar saturacion real se usan 8 consumidores,\n";
        std::cout << " porque el pool de VRAM tiene capacidad maxima de 5 slots.\n";
        std::cout << "============================================================\n";
    }

    if (escenario == 5) {
        std::cout << " Nota: la prueba genera primero 50 jobs Premium.\n";
        std::cout << " Luego, tras 5000ms, genera jobs Free y despues Premium.\n";
        std::cout << "============================================================\n";
    }

    std::cout << "\n";

    sistema.ejecutar(productores, consumidores, jobs, modo);

    pausar();
}

void submenuEscenarios(const Configuracion& config) {
    int escenario;

    do {
        mostrarMenuEscenarios(config);

        escenario = leerEntero();

        if (escenario == -1) {
            std::cout << "\nOpcion invalida.\n";
            pausar();
            continue;
        }

        if (escenario != 0) {
            ejecutarEscenario(config, escenario);
        }

    } while (escenario != 0);
}

void iniciarMenu() {
    int opcion;

    do {
        mostrarMenuPrincipal();

        opcion = leerEntero();

        if (opcion == -1) {
            std::cout << "\nOpcion invalida.\n";
            pausar();
            continue;
        }

        if (opcion >= 1 && opcion <= 3) {
            Configuracion config = obtenerConfiguracion(opcion);
            submenuEscenarios(config);
        } else if (opcion == 0) {
            std::cout << "Finalizando programa...\n";
        } else {
            std::cout << "\nOpcion invalida.\n";
            pausar();
        }

    } while (opcion != 0);
}
