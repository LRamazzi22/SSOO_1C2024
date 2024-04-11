#ifndef ENTRADASALIDA_GESTOR_H_
#define ENTRADASALIDA_GESTOR_H_

/*
Este es el gestor de la Entrada y Salida, se declaran variables globales usando 
el extern asi todos los modulos de la Entrada Y Salida pueden utilizarlas. 
Las variables deben ser previamente declaradas en su modulo
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils/utiles.h>

//Variables Globales

extern t_log* logger;
extern t_config* config;

extern int entradasalida_cliente_memoria;
extern int entradasalida_cliente_kernel;

extern char* TIPO_INTERFAZ;
extern int TIEMPO_UNIDAD_TRABAJO;
extern char* IP_KERNEL;
extern char* PUERTO_KERNEL;
extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA;
extern char* PATH_BASE_DIALFS;
extern int BLOCK_SIZE;
extern int BLOCK_COUNT;






#endif