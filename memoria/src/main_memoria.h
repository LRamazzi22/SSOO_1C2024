#ifndef MAIN_MEMORIA_H_
#define MAIN_MEMORIA_H_

#include <memoria_gestor.h>
#include <inicializar_memoria.h>
#include <atender_mensajes.h>
#include <leer_pseudocodigo.h>

t_log* logger;
t_config* config;

int memoria_server;
int cpu_cliente;
int kernel_cliente;
int entradasalida_cliente;

char* PUERTO_ESCUCHA;
int TAM_MEMORIA;
int TAM_PAGINA;
char* PATH_INSTRUCCIONES;
int RETARDO_RESPUESTA;

t_dictionary* diccionario_de_instrucciones;
int cantidad_instrucciones;

#endif