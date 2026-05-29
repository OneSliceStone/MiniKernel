#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Se incluyen los headers necesarios para acceder a las estructuras y funciones de otros módulos
#include "pcb.h"
#include "queue.h"
#include "sync.h"
#include "metrics.h"

#define QUANTUM 3 // El tiempo máximo de ráfaga por turno (en segundos)

// Accedemos a la cola global definida en el main.c
extern queue_t ready_queue;

// Función que ejecutará cada hilo CPU
void* cpu_runtime(void* arg) {
    int cpu_id = *(int*)arg;
    free(arg); // Liberamos la memoria dinámica del ID heredado

    printf("[CPU %d] Inicializada y lista para procesar.\n", cpu_id);

    while (1) {
        // Extraer un proceso de la cola (si está vacía, aquí se duerme automáticamente)
        pcb_t current_process = dequeue(&ready_queue);

        // Cambiar el estado del proceso a EJECUTANDO
        current_process.state = STATE_RUNNING;
        
        // Calcular cuánto tiempo va a ejecutar en este turno
        int execution_time = (current_process.remaining_time > QUANTUM) ? QUANTUM : current_process.remaining_time;

        printf("[CPU %d] Ejecutando Proceso %d por %d s (Restante original: %d s)\n", 
               cpu_id, current_process.pid, execution_time, current_process.remaining_time);

        // Simular la ejecución en hardware durmiendo el hilo el tiempo calculado
        sleep(execution_time);

        // Actualizar el tiempo restante del proceso
        current_process.remaining_time -= execution_time;

        // Tomar decisiones de planificación (Round Robin)
        if (current_process.remaining_time > 0) {
            // El proceso no ha terminado: Context Switch (Re-formar en la cola)
            current_process.state = STATE_READY;
            printf("[CPU %d] Quantum expirado para Proceso %d. Re-encolando...\n", cpu_id, current_process.pid);
            enqueue(&ready_queue, current_process);
        } else {
            current_process.state = STATE_TERMINATED;
            printf("[CPU %d] Proceso %d FINALIZADO con exito\n", cpu_id, current_process.pid);
            
            // LLAMADA AL MÓDULO DE MÉTRICAS:
            register_completion(current_process);
        }
    }

    return NULL;
}