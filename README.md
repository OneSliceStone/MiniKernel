# MiniKernel - Simulador de Planificación de CPU

Este proyecto es un simulador de un "mini sistema operativo" que demuestra los conceptos fundamentales de la concurrencia, la sincronización de hilos y la planificación de procesos en el espacio de usuario. Está desarrollado en **C** utilizando la librería de hilos **POSIX (pthreads)**.

## Características Principales

- **Generador de Procesos:** Un hilo independiente que simula la llegada de procesos con ráfagas de CPU (`burst_time`), prioridades y tiempos de arribo aleatorios, representados mediante estructuras PCB.
- **CPUs Simuladas:** Múltiples hilos que actúan como núcleos de procesamiento, compitiendo de forma segura por los procesos de la cola.
- **Algoritmo Round Robin:** Planificación por turnos con un tiempo de Quantum definido para evitar la inanición de los procesos.
- **Control de Concurrencia:** Implementación segura del patrón Productor-Consumidor utilizando **Mutexes** y **Variables de Condición** para evitar condiciones de carrera en la cola de listos.
- **Monitor de Métricas:** Un hilo dedicado que calcula y despliega estadísticas en tiempo real como el *Throughput* y el *Turnaround Time* promedio.

## Estructura del Repositorio

El proyecto está organizado siguiendo el estándar de desarrollo estructurado en C:

```text
MiniKernel/
├── include/          # Archivos de cabecera (.h)
│   ├── pcb.h         # Estructura del Process Control Block
│   ├── queue.h       # Interfaz de la cola de listos
│   ├── sync.h        # Declaración de variables de sincronización
│   └── metrics.h     # Interfaz del módulo de rendimiento
├── src/              # Código fuente (.c)
│   ├── main.c        # Flujo principal y generador de procesos
│   ├── queue.c       # Lógica de la cola segura
│   ├── cpu.c         # Simulación de las CPUs y Round Robin
│   └── metrics.c     # Implementación del monitor de estadísticas
├── tests/            # Pruebas unitarias
├── Makefile          # Automatización de la compilación
└── README.md         # Documentación del proyecto
 ```

## Ejecución del Programa

Para la correcta ejecuión del programa desde Linux, es necesario acceder a *cd MiniKernel*, donde se debe compilar el proyecto con el comando *make*, para finalmente acceder a la ejecución del programa se ingresa el comando *./minikernel*.

En el caso de que se desee borrar los archivos temporales creados con *make*, se usa el comando *make clean*.

## Autor

**Nombre:** Isaac Muñoz
**Institución:** Escuela Politécnica Nacional / Facultad de Ingeniería en Sistemas
**Materia:** Sistemas Operativos 
