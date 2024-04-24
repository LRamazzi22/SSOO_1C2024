#ifndef ATENDER_MENSAJES_H
#define ATENDER_MENSAJES_H

#include <memoria_gestor.h>

void atender_cpu_memoria();
void enviar_instruccion(int);
void atender_kernel_memoria();
void atender_entradasalida_memoria();

#endif