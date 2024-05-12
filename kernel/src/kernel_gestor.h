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

typedef enum{
    EXITO,
    RECURSO_INVALIDO,
    INTERFAZ_INVALIDA,
    FUERA_DE_MEMORIA,
    FINALIZADO_POR_USUARIO

} razones_exit;
typedef struct 
{
    int PID;
    int quantum_restante;
    t_registros_cpu* registros_cpu_en_pcb;
    psw estado_proceso; // Agrego reg de estado, para identificar el estado del proceso.
    pthread_t hilo_quantum;
    t_list* lista_recursos_tomados;
    razones_exit razon_salida;

} pcb;

typedef struct 
{
    char* tipo_de_interfaz;
    int* cliente;
    pthread_mutex_t mutex_interfaz_siendo_usada;
    sem_t hay_proceso_en_bloqueados;
    sem_t se_puede_enviar_proceso;
    sem_t detener_planificacion_recibir_respuestas_IO;
    sem_t detener_planificacion_enviar_peticion_IO;
} nodo_de_diccionario_interfaz;

typedef struct 
{
    t_queue* cola_bloqueados;
    pthread_mutex_t mutex_para_cola_bloqueados;
    t_queue* cola_Variables;
    pthread_mutex_t mutex_para_cola_variables;
} nodo_de_diccionario_blocked;

typedef struct 
{
    t_queue* cola_bloqueados_recurso;
    int instancias;
    pthread_mutex_t mutex_cola_bloqueados_recurso;
} nodo_recursos;






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
extern t_dictionary* diccionario_recursos;
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

extern int espera_grado_multi;

extern sem_t hay_proceso_en_ready;
extern sem_t hay_proceso_en_new;
extern sem_t hay_proceso_en_exit;
extern sem_t multiprogramacion_permite_proceso_en_ready;

extern sem_t detener_planificacion_exit;
extern sem_t detener_planificacion_to_ready;
extern sem_t detener_planificacion_corto_plazo;
extern sem_t detener_planificacion_salida_cpu;

extern pthread_mutex_t mutex_cola_new;
extern pthread_mutex_t mutex_cola_ready;
extern pthread_mutex_t mutex_cola_exit;
extern pthread_mutex_t mutex_para_proceso_en_ejecucion;
extern pthread_mutex_t mutex_para_creacion_proceso;
extern pthread_mutex_t mutex_para_diccionario_entradasalida;
extern pthread_mutex_t mutex_para_diccionario_recursos;
extern pthread_mutex_t mutex_para_diccionario_blocked;
extern pthread_mutex_t mutex_para_eliminar_entradasalida;



#endif