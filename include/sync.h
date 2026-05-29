#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

// Este archivo define las variables globales de sincronización que se usarán en todo el proyecto
// También se puede decir que es la forma para controlar la concurrencia entre los hilos sin tener que pasar mutex 
// O condiciones como argumentos a cada función

// Mutex para proteger el acceso a la cola de listos
extern pthread_mutex_t ready_queue_mutex;

// Variable de condición para bloquear/despertar a las CPUs
extern pthread_cond_t ready_queue_cond;

#endif // SYNC_H