#ifndef ATENDER_MENSAJES_H
#define ATENDER_MENSAJES_H

#include <cpu_gestor.h>

void recibir_contexto_de_CPU(t_buffer*);
void atender_kernel_dispatch_sin_while();
void atender_kernel_interrupt();
void atender_memoria_cpu();
void atender_memoria_cpu_sin_while();
void atender_kernel_interrupt();
void atender_memoria_cpu();
void atender_memoria_cpu_sin_while();

#endif