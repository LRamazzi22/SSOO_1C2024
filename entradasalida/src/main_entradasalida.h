#ifndef MAIN_ENTRADASALIDA_H_
#define MAIN_ENTRADASALIDA_H_

#include <entradasalida_gestor.h>
#include <inicializar_entradasalida.h>
#include <atender_mensajes.h>
#include <interfaz_generica.h>

t_log* logger;
t_config* config;

int entradasalida_cliente_memoria;
int entradasalida_cliente_kernel;

int tipo_de_interfaz;

pthread_mutex_t mutex_para_interfaz;

char* TIPO_INTERFAZ;
int TIEMPO_UNIDAD_TRABAJO;
char* IP_KERNEL;
char* PUERTO_KERNEL;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
int BLOCK_SIZE;
int BLOCK_COUNT;


int definir_tipo_interfaz();

#endif