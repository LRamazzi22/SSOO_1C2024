#ifndef UTILS_UTILES_H_
#define UTILS_UTILES_H_

#include <stdlib.h>
#include <stdio.h>


#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/queue.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>


typedef enum
{
	HANDSHAKE,
	PAQUETE,
	PROTOCOLO,
	PEDIR_INSTRUCCION,
    CREAR_PROCESO

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
t_paquete* crear_paquete(int);
void agregar_a_paquete(t_paquete*, void*, int);
void agregar_int_a_paquete(t_paquete*, int);
void agregar_string_a_paquete(t_paquete*, char*);
void* serializar_paquete(t_paquete*, int);
void enviar_paquete(t_paquete*, int);
void eliminar_paquete(t_paquete*);
void eliminar_buffer(t_buffer*);

int recibir_operacion(int);
t_buffer* recibir_buffer(int);
void* extraer_contenido_buffer(t_buffer*, t_log*);
int extraer_int_buffer(t_buffer*, t_log*);
char* extraer_string_buffer(t_buffer*, t_log*);


//---------------------Estructuras para Ciclo de Instruccion--------------//

typedef struct{
    uint32_t PC; //Program Counter (no sabemos si hacerlo tipo extern porq tambien tiene q estar en los pcb's)
    uint8_t AX; //Registro generico
    uint8_t BX; // "       "
    uint8_t CX; // "       "
    uint8_t DX; // "       "
    uint32_t EAX; // "       "
    uint32_t EBX; // "       "
    uint32_t ECX; // "       "
    uint32_t EDX; // "       "
    uint32_t SI; //direccion de memoria de origen desde donde se copia un string
    uint32_t DI; //direccion de memoria de destino donde se copia un string

} t_registros_cpu;

typedef enum{
    SET,
    MOV_IN,
    MOV_OUT,
    SUM,
    SUB,
    JNZ,
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ, 
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE, 
    IO_FS_READ, 
    EXIT,
} t_nombre_instruccion; //Podriamos cambiar leer pseudocodigo para que haga una lista encadenada de "nodos" de tipo instruccion (ver abajo) y asi no tener que comparar
					 //strings a la hora de leer una instruccion (a chekear este disenioo)



#endif
