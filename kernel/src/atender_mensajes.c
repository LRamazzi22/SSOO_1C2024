#include <atender_mensajes.h>

void atender_memoria(){
    while (1) {
        printf("Esperando mensajes de Memoria\n");
		int cod_op = recibir_operacion(kernel_cliente_memoria);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernel_cliente_memoria, logger);
			break;
		case -1:
			log_error(logger, "Se desconceto la Memoria");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_cpu_dispatch(){
    while (1) {
        printf("Esperando mensajes de CPU Dispatch\n");
		int cod_op = recibir_operacion(kernel_cliente_dispatch);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernel_cliente_dispatch, logger);
			break;
		case -1:
			log_error(logger, "El CPU Dispatch se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_cpu_interrupt(){
    while (1) {
        printf("Esperando mensajes de CPU Interrupt\n");
		int cod_op = recibir_operacion(kernel_cliente_interrupt);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernel_cliente_interrupt, logger);
			break;
		case -1:
			log_error(logger, "El Kernel Interrupt se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_entradasalida_kernel(){
    while (1) {
        printf("Esperando mensajes de Entrada/Salida\n");
		int cod_op = recibir_operacion(entradasalida_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(entradasalida_cliente, logger);
			break;
		case -1:
			log_error(logger, "Entrada/Salida se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}