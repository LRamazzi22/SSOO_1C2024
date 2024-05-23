#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <kernel_gestor.h>
#include <inicializar_kernel.h>
#include <crear_pcb.h>
#include <planificador_corto_plazo.h>


void consola_kernel();
void validar_y_ejecutar_comando(char**);
void crear_proceso(void*);
void iniciar_planificacion();



#endif