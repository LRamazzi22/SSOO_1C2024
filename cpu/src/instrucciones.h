#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include <cpu_gestor.h>
#include <atender_mensajes.h>
#include <ciclo_cpu.h>
#include <MMU.h>

void set(char*, int);
void sum(char*, char*);
void sub(char*, char*);
void jnz(char*, int);
void mov_in(char*, char*);
bool mov_out(char*, char*);
int resize(int);
bool copy_string(int );
void std_read_write(char*, char*, char*, char*);
io_std* io_std_get_dir_fis(char*, char*, char*);
void* apuntar_a_registro (char*, int*);

#endif