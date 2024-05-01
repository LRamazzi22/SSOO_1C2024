#ifndef MAIN_KERNEL_H_
#define MAIN_KERNEL_H_

#include <kernel_gestor.h>
#include <inicializar_kernel.h>
#include <atender_mensajes.h>
#include <consola.h>
#include <atender_nuevas_interfaces.h>
#include <planificador_corto_plazo.h>


//Variables Globales

int pid_acumulado = 0;
int cantidad_de_proceso_en_ejecucion;

t_log* logger;
t_config* config;

int kernel_server;
int entradasalida_cliente;
int kernel_cliente_dispatch;
int kernel_cliente_interrupt;
int kernel_cliente_memoria;

t_dictionary* diccionario_entrada_salida;
pcb* proceso_en_ejecucion;


char* PUERTO_ESCUCHA;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* IP_CPU;
char* PUERTO_CPU_DISPATCH;
char* PUERTO_CPU_INTERRUPT;
char* ALGORITMO_PLANIFICACION;
int QUANTUM;
char** RECURSOS;
char** INSTANCIAS_RECURSOS;
int GRADO_MULTIPROGRAMACION;

t_queue* cola_new;
t_queue* cola_ready;
t_queue* cola_blocked;
t_queue* cola_blocked_ready;
t_queue* cola_exit;

//Semaforos
sem_t hay_proceso_en_ready;
sem_t hay_un_proceso_en_la_cpu;

pthread_mutex_t mutex_cola_ready;
pthread_mutex_t mutex_para_creacion_proceso;


#endif