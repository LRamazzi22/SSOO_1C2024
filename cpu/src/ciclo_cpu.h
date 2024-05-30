#ifndef CICLO_CPU_H_
#define CICLO_CPU_H_

#include <utils/utiles.h>
#include <cpu_gestor.h>
#include <atender_mensajes.h>

void ciclo();
void solicitar_instruccion(int);
int decodificar_instruccion();
void set(char*, int);
void sum(char*, char*);
void sub(char*, char*);
void jnz(char*, int);
int resize(int);
int ejecutar_instruccion (int);
void* apuntar_a_registro (char*, int*);
void cargar_registros_a_paquete(t_paquete* );
//t_list* tlb;

t_mmu_traduccion* traducir_direccion(int);
t_entrada_tlb* obtener_entrada_tlb(int, int);

#endif