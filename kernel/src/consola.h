#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <kernel_gestor.h>
#include <inicializar_kernel.h>
#include <crear_pcb.h>


typedef enum
{
	EJECUTAR_SCRIPT,
    INICIAR_PROCESO,
    FINALIZAR_PROCESO,
    DETENER_PLANIFICACION,
    INICIAR_PLANIFICACION,
    MULTIPROGRAMACION,
    PROCESO_ESTADO,
    NO_RECONOCIDO

} comandos;

void consola_kernel();
comandos validar_comando(char**);
void ejecutar_comando(comandos, char**);
int crear_proceso(char*);



#endif