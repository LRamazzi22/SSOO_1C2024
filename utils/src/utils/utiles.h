#ifndef UTILS_UTILES_H_
#define UTILS_UTILES_H_

#include <stdlib.h>
#include <stdio.h>


#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <commons/string.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>


typedef enum
{
	HANDSHAKE,
	PAQUETE,
	PROTOCOLO

} op_code;

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

//Funciones de Server

int iniciar_servidor(char*, t_log*);
int esperar_cliente(int, t_log*, char*);
void liberar_conexion(int);




//-------------------- Protoloco de Comunicacion --------------------//
void enviar_handshake(char*, int);
void crear_buffer(t_paquete*);
t_paquete* crear_paquete(op_code);
void agregar_a_paquete(t_paquete*, void*, int);
void* serializar_paquete(t_paquete*, int);
void enviar_paquete(t_paquete*, int);
void eliminar_paquete(t_paquete*);
void eliminar_buffer(t_buffer*);

int recibir_operacion(int);
t_buffer* recibir_buffer(int);
void* extraer_contenido_buffer(t_buffer*, t_log*);
int extraer_int_buffer(t_buffer*, t_log*);

#endif
