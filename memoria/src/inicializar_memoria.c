#include <inicializar_memoria.h>

void inicializar_memoria(){
    logger = iniciar_logger("./memoria.log", "Memoria_Logger", LOG_LEVEL_DEBUG, 1);
    logger_obligatorio = iniciar_logger("./memoriaObligatorio.log", "Memoria_Logger_Obligatorio", LOG_LEVEL_INFO,1);
    inicializar_config_memoria();
}

void inicializar_config_memoria(){
    config = iniciar_config("./memoria_config.config");
    

    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA"); 
    PATH_INSTRUCCIONES = config_get_string_value(config, "PATH_INSTRUCCIONES");
    RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");
}