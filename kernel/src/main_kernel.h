#ifndef MAIN_KERNEL_H_
#define MAIN_KERNEL_H_

#include <kernel_gestor.h>
#include <inicializar_kernel.h>
#include <atender_mensajes.h>


//Variables Globales

t_log* logger;
t_config* config;

int kernel_server;
int entradasalida_cliente;
int kernel_cliente_dispatch;
int kernel_cliente_interrupt;
int kernel_cliente_memoria;


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


#endif