#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "sync.h"

// Inicializar las variables globales de sincronización
pthread_mutex_t ready_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready_queue_cond = PTHREAD_COND_INITIALIZER;

void init_queue(queue_t* q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

int is_empty(queue_t* q) {
    return q->size == 0;
}

// INSERTAR: Operación segura para el Generador de Procesos
void enqueue(queue_t* q, pcb_t process) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->process = process;
    new_node->next = NULL;

    // --- ENTRADA A LA SECCIÓN CRÍTICA ---
    pthread_mutex_lock(&ready_queue_mutex);

    if (is_empty(q)) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    q->size++;

    printf("[PROCESO] Proceso %d agregado a la cola de listos. (Total: %d)\n", process.pid, q->size);

    // Despertar a UNA de las CPUs que esté durmiendo esperando procesos
    pthread_cond_signal(&ready_queue_cond);

    pthread_mutex_unlock(&ready_queue_mutex);
    // --- SALIDA DE LA SECCIÓN CRÍTICA ---
}

// EXTRAER: Operación segura para las CPUs simuladas
pcb_t dequeue(queue_t* q) {
    // --- ENTRADA A LA SECCIÓN CRÍTICA ---
    pthread_mutex_lock(&ready_queue_mutex);

    // Si la cola está vacía, la CPU se duerme de forma eficiente.
    // Usamos un bucle 'while' por seguridad ante despertares espurios.
    while (is_empty(q)) {
        pthread_cond_wait(&ready_queue_cond, &ready_queue_mutex);
    }

    // Al salir del wait, la CPU vuelve a poseer el mutex automáticamente
    node_t* temp = q->head;
    pcb_t process = temp->process;

    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    q->size--;

    free(temp); // Liberamos la memoria del nodo contenedor

    pthread_mutex_unlock(&ready_queue_mutex);
    // --- SALIDA DE LA SECCIÓN CRÍTICA ---

    return process;
}