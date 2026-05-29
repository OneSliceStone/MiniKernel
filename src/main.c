#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // Para manejo de hilos
#include <time.h> // Para manejo de tiempos y métricas

// Se incluyen los headers necesarios para acceder a las estructuras y funciones de otros módulos
#include "metrics.h"
#include "pcb.h"
#include "queue.h"
#include "sync.h"

#define N_CPUS 2 // Simularemos un procesador de 2 núcleos

queue_t ready_queue; // Cola global de procesos listos para ser ejecutados
time_t start_time; // Variable global para marcar el inicio de la simulación, usada para métricas

// Declaramos la función de la CPU que está en el otro archivo
void* cpu_runtime(void* arg);

void* process_generator(void* arg) {
    (void)arg; 
    int pid_counter = 1;
    printf("Hilo generador iniciado.\n");
    
    while (1) {
        int sleep_time = (rand() % 3) + 1; // Entre 1 y 3 segundos
        sleep(sleep_time);
        
        pcb_t new_process;
        new_process.pid = pid_counter++;
        new_process.burst_time = (rand() % 8) + 2; // Entre 2 y 9 segundos de ráfaga
        new_process.remaining_time = new_process.burst_time;
        new_process.priority = (rand() % 5) + 1;
        new_process.arrival_time = (int)(time(NULL) - start_time);
        new_process.state = STATE_READY;
        
        printf("\n[PROCESO] Nuevo Proceso -> PID: %d | Burst: %d s | Llegada: T+%d s\n", 
               new_process.pid, new_process.burst_time, new_process.arrival_time);
        
        enqueue(&ready_queue, new_process);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    start_time = time(NULL);
    
    printf(" ---- Iniciando Simulación del MiniKernel ----   \n");

    init_queue(&ready_queue);
    
    init_metrics(); // Inicializar contadores de métricas
    
    // Crear el hilo del Generador de Procesos
    pthread_t generator_thread;
    pthread_create(&generator_thread, NULL, process_generator, NULL);
    
    // Crear los hilos de las CPUs simuladas
    pthread_t cpu_threads[N_CPUS];
    for (int i = 0; i < N_CPUS; i++) {
        int* cpu_id = malloc(sizeof(int));
        *cpu_id = i + 1;
        pthread_create(&cpu_threads[i], NULL, cpu_runtime, cpu_id);
    }

    // Crear el hilo dedicado a reportar las Métricas
    pthread_t metrics_thread;
    if (pthread_create(&metrics_thread, NULL, metrics_runtime, NULL) != 0) {
        perror("Error al crear el hilo de métricas");
        return 1;
    }
    
    // Aumentemos el tiempo a 40 segundos para permitir que se generen más procesos y se puedan observar mejor 
    // las métricas
    printf("[MAIN] El sistema correrá por 40 segundos...\n");
    sleep(40);
    
    printf("\n[MAIN] Simulación terminada.\n");
    return 0;
}