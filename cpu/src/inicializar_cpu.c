#include <inicializar_cpu.h>

void inicializar_cpu(){
    logger = iniciar_logger("./cpu.log", "Cpu_Logger", LOG_LEVEL_INFO, 1);
    logger_obligatorio = iniciar_logger("./cpuObligatorio.log", "Cpu_Logger_Obligatorio", LOG_LEVEL_INFO,1);
    pid_en_ejecucion = 0;
    interrupcion_recibida = NO_INTERRUPCION;
    pid_de_interrupcion = 0;
    inicializar_config_cpu();

    tlb = list_create();
}

void inicializar_config_cpu(){
    config = iniciar_config("./cpu_config.config");

    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    CANTIDAD_ENTRADAS_TLB = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    ALGORITMO_TLB = config_get_string_value(config, "ALGORITMO_TLB");
}