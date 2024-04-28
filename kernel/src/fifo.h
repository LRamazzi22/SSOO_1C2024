#ifndef FIFO_H_
#define FIFO_H_

#include <kernel_gestor.h>
#include <atender_mensajes.h>

int algoritmo_fifo();
void serializar_registros_procesador (t_paquete* , t_registros_cpu* );
void algoritmo_round_robin();

#endif