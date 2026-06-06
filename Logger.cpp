#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

Logger::Logger(const std::string& nombreArchivo) {
    archivo.open(nombreArchivo, std::ios::out);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de log." << std::endl;
    }
}

Logger::~Logger() {
    if (archivo.is_open()) {
        archivo.close();
    }
}

std::string Logger::obtenerTimestamp() {
    auto ahora = std::chrono::system_clock::now();
    std::time_t tiempo = std::chrono::system_clock::to_time_t(ahora);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&tiempo), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

void Logger::registrar(const Job& job, const std::string& evento) {
    std::lock_guard<std::mutex> lock(mutexLog);

    if (archivo.is_open()) {
        archivo << "[" << obtenerTimestamp() << "]"
                << " - Job ID " << job.id
                << " - Prioridad " << prioridadToString(job.prioridad)
                << " - " << evento
                << std::endl;
    }
}
