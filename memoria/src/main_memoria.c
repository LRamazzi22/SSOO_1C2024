#include <main_memoria.h>

int main(int argc, char* argv[]) {

    t_log* logger;
	t_config* config;

    char* PUERTO_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_PAGINA;
    char* PATH_INSTRUCCIONES;
    int RETARDO_RESPUESTA;

    config = iniciar_config("./memoria_config.config");
    logger = iniciar_logger("./memoria.log", "Memoria_Logger", LOG_LEVEL_DEBUG);

    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA"); 
    PATH_INSTRUCCIONES = config_get_string_value(config, "PATH_INSTRUCCIONES");
    RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");

    //Main para crear servidor

    int server_fd = iniciar_servidor(PUERTO_ESCUCHA, logger);
	log_info(logger, "Servidor listo para recibir al CPU");
	int cliente_fd = esperar_cliente(server_fd, logger);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd, logger);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			//list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;


    //decir_hola("Memoria");
    //return 0;
}

/*
void iterator(char* value, t_log* logger) {
	log_info(logger,"%s", value);
}
*/