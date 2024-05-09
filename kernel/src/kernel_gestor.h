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
#include <readline/history.h>

typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCKED,
    EXIT_PROCESS
} psw;
typedef struct 
{
    int PID;
    int quantum_restante;
    t_registros_cpu* registros_cpu_en_pcb;
    psw estado_proceso; // Agrego reg de estado, para identificar el estado del proceso.
    pthread_t hilo_quantum;

} pcb;

typedef struct 
{
    char* tipo_de_interfaz;
    int* cliente;
    pthread_mutex_t mutex_interfaz_siendo_usada;
    sem_t hay_proceso_en_bloqueados;
    sem_t se_puede_enviar_proceso;
} nodo_de_diccionario_interfaz;

typedef struct 
{
    t_queue* cola_bloqueados;
    pthread_mutex_t mutex_para_cola_bloqueados;
    t_queue* cola_Variables;
    pthread_mutex_t mutex_para_cola_variables;
} nodo_de_diccionario_blocked;





//Variables Globales
extern int pid_acumulado;
extern int cantidad_de_proceso_en_ejecucion;
extern int pid_a_eliminar;


extern t_log* logger;
extern t_log* logger_obligatorio;
extern t_config* config;

extern int kernel_server;
extern int entradasalida_cliente;
extern int kernel_cliente_dispatch;
extern int kernel_cliente_interrupt;
extern int kernel_cliente_memoria;

extern t_dictionary* diccionario_entrada_salida;
extern pcb* proceso_en_ejecucion;

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
extern t_dictionary* diccionario_blocked;
extern t_queue* cola_exit;

extern bool permitir_planificacion;

extern sem_t hay_proceso_en_ready;
extern sem_t hay_proceso_en_new;
extern sem_t hay_proceso_en_exit;
extern sem_t multiprogramacion_permite_proceso_en_ready;

extern pthread_mutex_t mutex_cola_new;
extern pthread_mutex_t mutex_cola_ready;
extern pthread_mutex_t mutex_cola_exit;
extern pthread_mutex_t mutex_para_proceso_en_ejecucion;
extern pthread_mutex_t mutex_para_creacion_proceso;
extern pthread_mutex_t mutex_para_diccionario_entradasalida;
extern pthread_mutex_t mutex_para_diccionario_blocked;
extern pthread_mutex_t mutex_para_eliminar_entradasalida;



#endif