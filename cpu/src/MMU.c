#include <MMU.h>

int solicitar_marco(int pid, int num_de_pag){
    t_paquete* paquete = crear_paquete(PEDIR_MARCO);
    agregar_int_a_paquete(paquete, pid);
    agregar_int_a_paquete(paquete, num_de_pag);
    enviar_paquete(paquete,cpu_cliente_memoria);
    eliminar_paquete(paquete);
    int marco = recibir_marco();
    log_info(logger_obligatorio, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", pid, num_de_pag, marco);
    return marco;
}

direccion_fisica* traducir_dir_logica(int pid ,int dir_logica){
    direccion_fisica* nueva_dir_fisica = malloc(sizeof(direccion_fisica));

    float dir_log_float = dir_logica;
    int num_de_pag = floor(dir_log_float / tam_de_pags_memoria);
    nueva_dir_fisica ->desplazamiento = dir_logica - num_de_pag * tam_de_pags_memoria;

    nueva_dir_fisica ->marco = solicitar_marco(pid,num_de_pag);

    nueva_dir_fisica ->base = nueva_dir_fisica ->marco * tam_de_pags_memoria;

    nueva_dir_fisica ->dir_fisica_final = nueva_dir_fisica ->base + nueva_dir_fisica ->desplazamiento;

    nueva_dir_fisica ->num_de_pag_base = num_de_pag;

    return nueva_dir_fisica;
}



