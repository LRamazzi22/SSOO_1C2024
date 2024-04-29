#ifndef ATENDER_NUEVAS_INTERFACES_H_
#define ATENDER_NUEVAS_INTERFACES_H_

#include <kernel_gestor.h>
#include <atender_mensajes.h>

typedef struct {
    char* nombre;
    int cliente;
} nombre_y_cliente;


void atender_nueva_interfaz(void*);
void atender_cierre_de_interfaz(void*);
void atender_las_nuevas_interfaces();




#endif