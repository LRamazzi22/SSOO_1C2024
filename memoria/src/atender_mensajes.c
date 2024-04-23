#include <atender_mensajes.h>

void atender_cpu_memoria(){
	t_buffer* buffer;
	char* mensaje;
    while (1) {
		log_info(logger, "Esperando mensajes de CPU");
		int cod_op = recibir_operacion(cpu_cliente);
		switch (cod_op) {
		case HANDSHAKE:
			buffer = recibir_buffer(cpu_cliente);
			mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case PROTOCOLO:
			buffer = recibir_buffer(cpu_cliente);
			int num = extraer_int_buffer(buffer, logger);
			mensaje = extraer_string_buffer(buffer, logger);
			printf("El numero es: %d \n", num);
			printf("%s", mensaje);
			free(mensaje);
			break;
		case PEDIR_INSTRUCCION:
			buffer = recibir_buffer(cpu_cliente);
			int pc = extraer_int_buffer(buffer,logger);
			t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION);
			char * instruccion = list_get(lista_de_instrucciones,pc);
			agregar_string_a_paquete(paquete, instruccion);
			enviar_paquete(paquete, cpu_cliente);
    		eliminar_paquete(paquete);
			free(instruccion);
			break;
		case -1:
			log_error(logger, "El CPU se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void atender_kernel_memoria(){
    while (1) {
        log_info(logger, "Esperando mensajes de Kernel");
		int cod_op = recibir_operacion(kernel_cliente);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(kernel_cliente);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case -1:
			log_error(logger, "El Kernel se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void atender_entradasalida_memoria(){
    while (1) {
        log_info(logger, "Esperando mensajes de Entrada/Salida");
		int cod_op = recibir_operacion(entradasalida_cliente);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(entradasalida_cliente);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case -1:
			log_error(logger, "La Entrada/Salida se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}