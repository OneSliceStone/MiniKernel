#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

// Nodo de la lista enlazada que almacena un PCB
typedef struct node {
    pcb_t process;
    struct node* next;
} node_t;

// Estructura de la cola
typedef struct {
    node_t* head;
    node_t* tail;
    int size;
} queue_t;

// Funciones principales de la cola

// Función para iniciar una cola
void init_queue(queue_t* q);

//Función para encolar un proceso (agregar al final)
void enqueue(queue_t* q, pcb_t process);

// Función para desencolar un proceso (extraer del frente)
pcb_t dequeue(queue_t* q);

// Función para verificar si la cola está vacía
int is_empty(queue_t* q);

#endif // QUEUE_H