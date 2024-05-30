#include <MMU.h>

int solicitar_marco(int pid, int num_de_pag){
    t_paquete* paquete = crear_paquete(PEDIR_MARCO);
    agregar_int_a_paquete(paquete, pid);
    agregar_int_a_paquete(paquete, num_de_pag);
    enviar_paquete(paquete,cpu_cliente_memoria);
    eliminar_paquete(paquete);
    int marco = recibir_marco();
    return marco;
}