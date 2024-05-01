#ifndef CPU_GESTOR_H_
#define CPU_GESTOR_H_

/*
Este es el gestor del CPU, se declaran variables globales usando el extern asi todos los
modulos del CPU pueden utilizarlas. Las variables deben ser previamente declaradas en su modulo
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils/utiles.h>

typedef enum{
    FINALIZAR,
    SEGUIR_EJECUTANDO,
    SLEEP_GEN
} salidas_cpu;

//Variables Globales

extern int cpu_server_dispatch;
extern int cpu_server_interrupt;
extern int cpu_cliente_memoria;
extern int kernel_cliente_dispatch;
extern int kernel_cliente_interrupt;

extern t_log* logger;
extern t_config* config;

extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA;
extern char* PUERTO_ESCUCHA_DISPATCH;
extern char* PUERTO_ESCUCHA_INTERRUPT;
extern int CANTIDAD_ENTRADAS_TLB;
extern char* ALGORITMO_TLB;

extern char* instruccion_a_decodificar;
extern char** instruccion_separada;
extern int pid_en_ejecucion;

extern t_registros_cpu* los_registros_de_la_cpu;

#endif