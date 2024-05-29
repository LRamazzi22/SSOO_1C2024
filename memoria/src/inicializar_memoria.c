#include <inicializar_memoria.h>

void inicializar_memoria(){
    logger = iniciar_logger("./memoria.log", "Memoria_Logger", LOG_LEVEL_DEBUG, 1);
    logger_obligatorio = iniciar_logger("./memoriaObligatorio.log", "Memoria_Logger_Obligatorio", LOG_LEVEL_INFO,1);
    inicializar_config_memoria();


    if(TAM_MEMORIA % TAM_PAGINA != 0){
		log_error(logger, "Tamanio de memoria no es multiplo de tamanio de pagina");
		exit(EXIT_FAILURE);
	}
	memoria_de_usuario = malloc(TAM_MEMORIA);

    cant_marcos = TAM_MEMORIA/TAM_PAGINA;
    int cant_marcos_para_bitarray = ceil(cant_marcos/8);

    puntero_a_bits_de_los_marcos = malloc(cant_marcos_para_bitarray);
    marcos_de_memoria_libres = bitarray_create_with_mode(puntero_a_bits_de_los_marcos,cant_marcos_para_bitarray,LSB_FIRST);

    diccionario_de_instrucciones = dictionary_create();
    diccionario_de_tdp = dictionary_create();
}

void inicializar_config_memoria(){
    config = iniciar_config("./memoria_config.config");
    

    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA"); 
    PATH_INSTRUCCIONES = config_get_string_value(config, "PATH_INSTRUCCIONES");
    RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");
}