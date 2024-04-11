#ifndef ATENDER_MENSAJES_H
#define ATENDER_MENSAJES_H

#include <cpu_gestor.h>

void atender_kernel_dispatch();
void atender_kernel_interrupt();
void atender_memoria_cpu();

#endif