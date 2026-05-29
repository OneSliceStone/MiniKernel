#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "metrics.h"

// Variables globales protegidas para el cálculo
static int total_completed_processes = 0;
static int total_turnaround_time = 0;

// Mutex para evitar colisiones al actualizar las métricas desde las CPUs
static pthread_mutex_t metrics_mutex = PTHREAD_MUTEX_INITIALIZER;

// Necesitamos el tiempo de inicio global para calcular marcas de tiempo relativas
extern time_t start_time;

void init_metrics() {
    total_completed_processes = 0;
    total_turnaround_time = 0;
}

void register_completion(pcb_t process) {
    pthread_mutex_lock(&metrics_mutex);

    total_completed_processes++;
    
    // Turnaround Time = Tiempo_Final - Tiempo_Llegada
    int current_time = (int)(time(NULL) - start_time);
    int turnaround = current_time - process.arrival_time;
    total_turnaround_time += turnaround;

    printf("[METRICAS] Proceso %d registrado. Turnaround individual: %d s\n", process.pid, turnaround);

    pthread_mutex_unlock(&metrics_mutex);
}

void* metrics_runtime(void* arg) {
    (void)arg;
    printf("[METRICAS] Hilo monitor de rendimiento iniciado.\n");

    while (1) {
        // El hilo despierta cada 5 segundos para imprimir el reporte de rendimiento
        sleep(5);

        pthread_mutex_lock(&metrics_mutex);

        double throughput = 0.0;
        double avg_turnaround = 0.0;
        int elapsed_time = (int)(time(NULL) - start_time);

        if (elapsed_time > 0) {
            // Throughput = Procesos completados / Tiempo total transcurrido
            throughput = (double)total_completed_processes / elapsed_time;
        }

        if (total_completed_processes > 0) {
            // Turnaround promedio = Suma de turnarounds / Total de procesos
            avg_turnaround = (double)total_turnaround_time / total_completed_processes;
        }

        // Imprimir el reporte de rendimiento cada 5 segundos
        printf("\n=========================================\n");
        printf("   REPORTE DE RENDIMIENTO (T+%d s)       \n", elapsed_time);
        printf("-----------------------------------------\n");
        printf(" Procesos Completados: %d\n", total_completed_processes);
        printf(" Throughput Actual:    %.2f proc/seg\n", throughput);
        printf(" Turnaround Promedio:  %.2f seg\n", avg_turnaround);
        printf("=========================================\n\n");

        pthread_mutex_unlock(&metrics_mutex);
    }

    return NULL;
}