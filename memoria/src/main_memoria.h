#ifndef MAIN_MEMORIA_H_
#define MAIN_MEMORIA_H_

#include <memoria_gestor.h>
#include <inicializar_memoria.h>
#include <atender_mensajes.h>
#include <leer_pseudocodigo.h>
#include <atender_nuevas_interfaces.h>

t_log* logger;
t_log* logger_obligatorio;
t_config* config;

pthread_mutex_t mutex_para_leer_pseudo;
pthread_mutex_t mutex_para_diccionario_instrucciones;

int memoria_server;
int cpu_cliente;
int kernel_cliente;

char* PUERTO_ESCUCHA;
int TAM_MEMORIA;
int TAM_PAGINA;
char* PATH_INSTRUCCIONES;
int RETARDO_RESPUESTA;

t_dictionary* diccionario_de_instrucciones;


#endif