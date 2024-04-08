#ifndef MAIN_KERNEL_H_
#define MAIN_KERNEL_H_

#include <kernel_gestor.h>
#include <inicializar_kernel.h>


//Variables Globales

t_log* logger;
t_config* config;

int PUERTO_ESCUCHA;
char* IP_MEMORIA;
int PUERTO_MEMORIA;
char* IP_CPU;
int PUERTO_CPU_DISPATCH;
int PUERTO_CPU_INTERRUPT;
char* ALGORITMO_PLANIFICACION;
int QUANTUM;
char** RECURSOS;
char** INSTANCIAS_RECURSOS;
int GRADO_MULTIPROGRAMACION;


#endif