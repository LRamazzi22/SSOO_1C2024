#ifndef PLANIFICADOR_LARGO_PLAZO_H_
#define PLANIFICADOR_LARGO_PLAZO_H_

#include <kernel_gestor.h>


void iniciar_planificacion_largo_plazo();
void planificador_new_to_ready();
void planificador_exit();
void borrar_registros_pcb(pcb*);

#endif