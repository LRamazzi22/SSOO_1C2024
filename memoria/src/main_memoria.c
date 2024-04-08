#include <main_memoria.h>

int main(int argc, char* argv[]) {

    inicializar_memoria();

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