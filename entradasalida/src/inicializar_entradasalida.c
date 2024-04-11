#include <inicializar_entradasalida.h>

void inicializar_entradasalida(){
    logger = iniciar_logger("./entradasalida.log", "EntradaSalida_Logger", LOG_LEVEL_INFO);
    inicializar_config_entradasalida();
}

void inicializar_config_entradasalida(){

    config = iniciar_config("./entradasalida_config.config");
    
    TIPO_INTERFAZ = config_get_string_value(config, "TIPO_INTERFAZ");
    TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
    IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    PUERTO_KERNEL = config_get_string_value(config, "PUERTO_KERNEL");
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA =  config_get_string_value(config, "PUERTO_MEMORIA");
    PATH_BASE_DIALFS = config_get_string_value(config, "PATH_BASE_DIALFS");
    BLOCK_SIZE = config_get_int_value(config, "BLOCK_SIZE");
    BLOCK_COUNT = config_get_int_value(config, "BLOCK_COUNT");
}