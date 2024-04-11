#ifndef ATENDER_MENSAJES_H
#define ATENDER_MENSAJES_H

#include <kernel_gestor.h>

void atender_memoria();
void atender_cpu_dispatch();
void atender_cpu_interrupt();
void atender_entradasalida_kernel();

#endif