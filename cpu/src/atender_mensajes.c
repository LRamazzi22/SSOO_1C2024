#include <atender_mensajes.h>

void atender_kernel_dispatch(){
    while (1) {
        printf("Esperando mensajes de Kernel\n");
		int cod_op = recibir_operacion(kernel_cliente_dispatch);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernel_cliente_dispatch, logger);
			break;
		case -1:
			log_error(logger, "El Kernel Dispatch se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}    
}

void atender_kernel_interrupt(){
    while (1) {
        printf("Esperando mensajes de Kernel cliente interrupt\n");
		int cod_op = recibir_operacion(kernel_cliente_interrupt);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernel_cliente_interrupt, logger);
			break;
		case -1:
			log_error(logger, "El Kernel Interrupt se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_memoria_cpu(){
    while (1) {
        printf("Esperando mensajes de la Memoria\n");
		int cod_op = recibir_operacion(cpu_cliente_memoria);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cpu_cliente_memoria, logger);
			break;
		case -1:
			log_error(logger, "La Memoria se desconecto");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

