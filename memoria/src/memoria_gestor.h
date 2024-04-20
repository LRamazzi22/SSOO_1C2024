#ifndef MEMORIA_GESTOR_H_
#define MEMORIA_GESTOR_H_

/*
Este es el gestor de la Memoria, se declaran variables globales usando el extern asi todos los
modulos de la Memoria pueden utilizarlas. Las variables deben ser previamente declaradas en su modulo
*/

#include <stdlib.h>
#include <stdio.h>
#include <utils/utiles.h>

//Variables Globales

extern t_log* logger;
extern t_config* config;

extern int memoria_server;
extern int cpu_cliente;
extern int kernel_cliente;
extern int entradasalida_cliente;

extern char* PUERTO_ESCUCHA;
extern int TAM_MEMORIA;
extern int TAM_PAGINA;
extern char* PATH_INSTRUCCIONES;
extern int RETARDO_RESPUESTA;



typedef struct {
    char ** lista_de_instrucciones;
    int cantidad_instrucciones;
}memoriaInstrucciones;

extern memoriaInstrucciones* instrucciones;

#endif