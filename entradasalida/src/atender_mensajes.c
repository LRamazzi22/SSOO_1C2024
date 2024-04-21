#include <atender_mensajes.h>

void atender_kernel_entradasalida(){
     while (1) {
		log_info(logger, "Esperando mensajes de Kernel");
		int cod_op = recibir_operacion(entradasalida_cliente_kernel);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(entradasalida_cliente_kernel);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case -1:
			log_error(logger, "Se desconceto el Kernel");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_memoria_entradasalida(){
    while (1) {
		log_info(logger, "Esperando mensajes de Memoria");
		int cod_op = recibir_operacion(entradasalida_cliente_memoria);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(entradasalida_cliente_memoria);
			char* mensaje = extraer_string_buffer(buffer, logger);
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