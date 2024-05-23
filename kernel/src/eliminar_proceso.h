#ifndef ELIMINAR_PROCESO_H_
#define ELIMINAR_PROCESO_H_

#include <kernel_gestor.h>

void mandar_a_exit(pcb*);
void eliminar_el_proceso(pcb*);
void borrar_registros_pcb(pcb*);


#endif