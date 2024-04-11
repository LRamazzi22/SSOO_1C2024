#ifndef UTILS_UTILES_H_
#define UTILS_UTILES_H_

#include <stdlib.h>
#include <stdio.h>


#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>


typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

void decir_hola(char*);
t_config* iniciar_config(char*);
t_log* iniciar_logger(char*, char*, int);
void terminar_programa(t_log*, t_config*);

//Funciones de Cliente
int crear_conexion(char*, char*);
void enviar_mensaje(char*, int);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete*, void*, int);
void enviar_paquete(t_paquete*, int);
void liberar_conexion(int);
void eliminar_paquete(t_paquete*);


//Funciones de Server

void* recibir_buffer(int*, int);
int iniciar_servidor(char*, t_log*);
int esperar_cliente(int, t_log*, char*);
t_list* recibir_paquete(int);
void recibir_mensaje(int, t_log*);
int recibir_operacion(int);




#endif
