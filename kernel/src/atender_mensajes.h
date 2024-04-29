#ifndef ATENDER_MENSAJES_H
#define ATENDER_MENSAJES_H

#include <kernel_gestor.h>
#include <eliminar_elemento_de_cola.h>

typedef struct{
    pcb* pcb_proceso;
    int cliente;
} estructura_para_atender_IO;

void atender_memoria();
int recibir_PC_memoria();
void atender_cpu_dispatch();
nodo_de_diccionario_interfaz* comprobrar_existencia_de_interfaz(pcb*, char* ,char* );
void recibir_contexto_de_ejecucion(t_buffer* ,pcb*); 
void atender_cpu_interrupt();
void atender_entradasalida_kernel(void*);


#endif