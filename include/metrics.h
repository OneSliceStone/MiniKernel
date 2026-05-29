#ifndef METRICS_H
#define METRICS_H

#include "pcb.h"

// Inicializa los contadores globales de rendimiento
void init_metrics();

// Registra los datos de un proceso cuando este termina su ejecución en la CPU
void register_completion(pcb_t process);

// Función que ejecutará el hilo dedicado a mostrar las estadísticas
void* metrics_runtime(void* arg);

#endif // METRICS_H