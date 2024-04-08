#ifndef KERNEL_GESTOR_H_
#define KERNEL_GESTOR_H_

/*
Este es el gestor del Kernel, se declaran variables globales usando el extern asi todos los
modulos del kernel pueden utilizarlas. Las variables deben ser previamente declaradas en su modulo
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils/utiles.h>

//Variables Globales

extern t_log* logger;
extern t_config* config;

extern int PUERTO_ESCUCHA;
extern char* IP_MEMORIA;
extern int PUERTO_MEMORIA;
extern char* IP_CPU;
extern int PUERTO_CPU_DISPATCH;
extern int PUERTO_CPU_INTERRUPT;
extern char* ALGORITMO_PLANIFICACION;
extern int QUANTUM;
extern char** RECURSOS;
extern char** INSTANCIAS_RECURSOS;
extern int GRADO_MULTIPROGRAMACION;



#endif