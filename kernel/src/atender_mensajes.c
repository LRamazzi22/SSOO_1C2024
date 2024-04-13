#include <atender_mensajes.h>

void atender_memoria(){
    while (1) {
		log_info(logger, "Esperando mensajes de Memoria");
		int cod_op = recibir_operacion(kernel_cliente_memoria);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(kernel_cliente_memoria);
			char* mensaje = extraer_contenido_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
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
		log_info(logger, "Esperando mensajes de CPU Dispatch");
		int cod_op = recibir_operacion(kernel_cliente_dispatch);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(kernel_cliente_dispatch);
			char* mensaje = extraer_contenido_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
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
		log_info(logger, "Esperando mensajes de CPU Interrupt");
		int cod_op = recibir_operacion(kernel_cliente_interrupt);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(kernel_cliente_interrupt);
			char* mensaje = extraer_contenido_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
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
		log_info(logger, "Esperando mensajes de Entrada/Salida");
		int cod_op = recibir_operacion(entradasalida_cliente);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(entradasalida_cliente);
			char* mensaje = extraer_contenido_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
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