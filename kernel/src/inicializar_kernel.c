#include <inicializar_kernel.h>

void inicializar_kernel(){
    logger = iniciar_logger("./kernel.log", "Kernel_Logger", LOG_LEVEL_INFO);
    inicializar_config_kernel();
}

void inicializar_config_kernel(){
    config = iniciar_config("./kernel_config.config");

    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    IP_CPU = config_get_string_value(config,"IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    ALGORITMO_PLANIFICACION = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
    QUANTUM = config_get_int_value(config, "QUANTUM");
    RECURSOS = config_get_array_value(config,"RECURSOS");
    INSTANCIAS_RECURSOS = config_get_array_value(config,"INSTANCIAS_RECURSOS");
    GRADO_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
}
