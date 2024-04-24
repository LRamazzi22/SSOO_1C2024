#ifndef ATENDER_MENSAJES_H
#define ATENDER_MENSAJES_H

#include <memoria_gestor.h>
#include <leer_pseudocodigo.h>

void atender_cpu_memoria();
void enviar_instruccion(int);
void atender_kernel_memoria();
void enviar_program_counter(int);
void atender_entradasalida_memoria();

#endif