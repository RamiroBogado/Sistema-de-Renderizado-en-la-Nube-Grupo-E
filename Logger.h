#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>
#include "Job.h"

struct Logger {
    std::ofstream archivo;
    std::mutex mutexLog;

    Logger(const std::string& nombreArchivo);
    ~Logger();

    std::string obtenerTimestamp();
    void registrar(const Job& job, const std::string& evento);
};

#endif
