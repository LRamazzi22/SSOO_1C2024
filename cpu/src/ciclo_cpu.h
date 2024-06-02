#ifndef CICLO_CPU_H_
#define CICLO_CPU_H_

#include <utils/utiles.h>
#include <cpu_gestor.h>
#include <atender_mensajes.h>
#include <MMU.h>

void ciclo();
void solicitar_instruccion(int);
int decodificar_instruccion();
void set(char*, int);
void sum(char*, char*);
void sub(char*, char*);
void jnz(char*, int);
void mov_in(char*, char*);
void mov_out(char*, char*);
int resize(int);
void copy_string(int );
int ejecutar_instruccion (int);
void* apuntar_a_registro (char*, int*);
void cargar_registros_a_paquete(t_paquete* );

#endif