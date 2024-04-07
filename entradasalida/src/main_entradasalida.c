#include <main_entradasalida.h>

int main(int argc, char* argv[]) {
     
    t_log* logger;
	t_config* config;


    char* TIPO_INTERFAZ;
    int TIEMPO_UNIDAD_TRABAJO;
    char* IP_KERNEL;
    int PUERTO_KERNEL;
    char* IP_MEMORIA;
    int PUERTO_MEMORIA;
    char* PATH_BASE_DIALFS;
    int BLOCK_SIZE;
    int BLOCK_COUNT;

    config = iniciar_config("./entradasalida_config.config");
    logger = iniciar_logger("./entradasalida.log", "EntradaSalida_Logger", LOG_LEVEL_INFO);

    TIPO_INTERFAZ = config_get_string_value(config, "TIPO_INTERFAZ");
    TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
    IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    PUERTO_KERNEL config_get_int_value(config, "PUERTO_KERNEL");
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA =  config_get_int_value(config, "PUERTO_MEMORIA");
    PATH_BASE_DIALFS = config_get_string_value(config, "PATH_BASE_DIALFS");
    BLOCK_SIZE = config_get_int_value(config, "BLOCK_SIZE");
    BLOCK_COUNT = config_get_int_value(config, "BLOCK_COUNT");

    decir_hola("una Interfaz de Entrada/Salida");
    return 0;
    
}
