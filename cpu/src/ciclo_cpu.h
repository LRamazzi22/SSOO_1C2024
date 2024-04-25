#ifndef CICLO_CPU_H_
#define CICLO_CPU_H_

#include <utils/utiles.h>
#include <cpu_gestor.h>

void ciclo();
void solicitar_instruccion(int);
int decodificar_instruccion();
int ejecutar_instruccion (int );
void* apuntar_a_registro (char *, void*);

#endif