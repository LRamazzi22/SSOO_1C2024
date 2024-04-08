#ifndef MAIN_ENTRADASALIDA_H_
#define MAIN_ENTRADASALIDA_H_

#include <entradasalida_gestor.h>
#include <inicializar_entradasalida.h>

t_log* logger;
t_config* config;

char* TIPO_INTERFAZ;
int TIEMPO_UNIDAD_TRABAJO;
char* IP_KERNEL;
int PUERTO_KERNEL;
char* IP_MEMORIA;
int PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
int BLOCK_SIZE;
int BLOCK_COUNT;

#endif