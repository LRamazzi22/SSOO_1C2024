#ifndef KERNEL_GESTOR_H_
#define KERNEL_GESTOR_H_

/*
Este es el gestor del Kernel, se declaran variables globales usando el extern asi todos los
modulos del kernel pueden utilizarlas. Las variables deben ser previamente declaradas en su modulo
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils/utiles.h>

#include <readline/readline.h>

typedef enum {
    NEW = 0,
    READY = 1,
    EXEC = 2,
    BLOCKED = 3,
    EXIT_PROCESS = 4
} psw;
typedef struct 
{
    int PID;
    int quantum_restante;
    t_registros_cpu* registros_cpu_en_pcb;
    psw estado_proceso; // Agrego reg de estado, para identificar el estado del proceso.

} pcb;



//Variables Globales
extern int pid_acumulado;
extern int grado_multiprogramacion_variable;

extern pthread_mutex_t mutex_para_creacion_proceso;

extern t_log* logger;
extern t_config* config;

extern int kernel_server;
extern int entradasalida_cliente;
extern int kernel_cliente_dispatch;
extern int kernel_cliente_interrupt;
extern int kernel_cliente_memoria;

extern char* PUERTO_ESCUCHA;
extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA;
extern char* IP_CPU;
extern char* PUERTO_CPU_DISPATCH;
extern char* PUERTO_CPU_INTERRUPT;
extern char* ALGORITMO_PLANIFICACION;
extern int QUANTUM;
extern char** RECURSOS;
extern char** INSTANCIAS_RECURSOS;
extern int GRADO_MULTIPROGRAMACION;

extern t_queue* cola_new;
extern t_queue* cola_ready;
extern t_queue* cola_blocked;
extern t_queue* cola_exit;



#endif