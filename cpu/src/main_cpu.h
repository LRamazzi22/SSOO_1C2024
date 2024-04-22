#ifndef MAIN_CPU_H_
#define MAIN_CPU_H_

#include <cpu_gestor.h>
#include <inicializar_cpu.h>
#include <atender_mensajes.h>

t_log* logger;
t_config* config;

int cpu_server_dispatch;
int cpu_server_interrupt;
int cpu_cliente_memoria;
int kernel_cliente_dispatch;
int kernel_cliente_interrupt;
    
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* PUERTO_ESCUCHA_DISPATCH;
char* PUERTO_ESCUCHA_INTERRUPT;
int CANTIDAD_ENTRADAS_TLB;
char* ALGORITMO_TLB;

char* instruccion_a_decodificar; //Variable donde se almacena la instruccion recibida por la memoria, es necesario decodificarla

registros_cpu los_registros_de_la_cpu;

void prueba_de_protocolo();
void solicitar_instruccion(int );

#endif


