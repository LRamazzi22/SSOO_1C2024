#ifndef MAIN_CPU_H_
#define MAIN_CPU_H_

#include <cpu_gestor.h>
#include <inicializar_cpu.h>

t_log* logger;
t_config* config;
    
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
int PUERTO_ESCUCHA_DISPATCH;
int PUERTO_ESCUCHA_INTERRUPT;
int CANTIDAD_ENTRADAS_TLB;
char* ALGORITMO_TLB;

#endif


