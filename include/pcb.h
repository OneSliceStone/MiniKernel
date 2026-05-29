#ifndef PCB_H
#define PCB_H

// Definición de los estados del proceso para mayor claridad
// También esta estructura se utilizará en el main para
typedef enum {
    STATE_NEW,
    STATE_READY,
    STATE_RUNNING,
    STATE_TERMINATED
} process_state_t;

// Estructura del PCB de acuerdo a lo solicitado en el enunciado
typedef struct {
    int pid;            // Identificador del proceso 
    int burst_time;     // Tiempo total de CPU requerido 
    int remaining_time; // Tiempo restante de ejecución 
    int priority;       // Prioridad del proceso 
    int arrival_time;   // Tiempo de llegada al sistema 
    int state;          // Estado actual del proceso 
} pcb_t;

#endif // PCB_Hx