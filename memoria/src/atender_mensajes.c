#include <atender_mensajes.h>

void atender_cpu_memoria(){
    while (1) {
        printf("Esperando mensajes de CPU\n");
		int cod_op = recibir_operacion(cpu_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cpu_cliente, logger);
			break;
		case -1:
			log_error(logger, "El CPU se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void atender_kernel_memoria(){
    while (1) {
        printf("Esperando mensajes de Kernel\n");
		int cod_op = recibir_operacion(kernel_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernel_cliente, logger);
			break;
		case -1:
			log_error(logger, "El Kernel se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void atender_entradasalida_memoria(){
    while (1) {
        printf("Esperando mensajes de la Entrada/Salida \n");
		int cod_op = recibir_operacion(entradasalida_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(entradasalida_cliente, logger);
			break;
		case -1:
			log_error(logger, "La Entrada Sali se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}