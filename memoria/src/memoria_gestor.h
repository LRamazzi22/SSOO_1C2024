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

extern char* PUERTO_ESCUCHA;
extern int TAM_MEMORIA;
extern int TAM_PAGINA;
extern char* PATH_INSTRUCCIONES;
extern int RETARDO_RESPUESTA;






#endif