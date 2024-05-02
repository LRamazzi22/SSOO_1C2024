#include <atender_mensajes.h>

// Atender mensajes enviados por el Kernel



void recibir_contexto_de_CPU(t_buffer* buffer) {
	buffer = recibir_buffer(kernel_cliente_dispatch);
	pid_en_ejecucion = extraer_int_buffer(buffer,logger);
	*los_registros_de_la_cpu->PC = extraer_uint32_buffer(buffer, logger);
	*los_registros_de_la_cpu->AX = extraer_uint8_buffer(buffer, logger);	
	*los_registros_de_la_cpu->BX = extraer_uint8_buffer(buffer, logger);
	*los_registros_de_la_cpu->CX = extraer_uint8_buffer(buffer, logger);
	*los_registros_de_la_cpu->DX = extraer_uint8_buffer(buffer, logger);
	*los_registros_de_la_cpu->EAX = extraer_uint32_buffer(buffer, logger);
	*los_registros_de_la_cpu->EBX = extraer_uint32_buffer(buffer, logger);
	*los_registros_de_la_cpu->ECX = extraer_uint32_buffer(buffer, logger);
	*los_registros_de_la_cpu->EDX = extraer_uint32_buffer(buffer, logger);
	*los_registros_de_la_cpu->SI = extraer_uint32_buffer(buffer, logger);
	*los_registros_de_la_cpu->DI = extraer_uint32_buffer(buffer, logger);

	
}



void atender_kernel_dispatch_sin_while(){
	log_info(logger, "Esperando mensajes de Kernel");
	t_buffer* buffer = NULL;
	int cod_op = recibir_operacion(kernel_cliente_dispatch);
	switch (cod_op) {
	case HANDSHAKE:
		buffer = recibir_buffer(kernel_cliente_dispatch);
		char* mensaje = extraer_string_buffer(buffer, logger);
		printf("Recibi un handshake de %s, como cliente", mensaje);
		free(mensaje);
		break;
	case INICIAR_EXEC:
		recibir_contexto_de_CPU(buffer);
		break;
	case -1:
		log_error(logger, "El Kernel Dispatch se desconecto");
		exit(EXIT_FAILURE);
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
}

void atender_kernel_interrupt(){
    while (1) {
        log_info(logger, "Esperando mensajes de de Kernel cliente interrupt");
		int cod_op = recibir_operacion(kernel_cliente_interrupt);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(kernel_cliente_interrupt);
			char* mensaje = extraer_string_buffer(buffer, logger);
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

void atender_memoria_cpu(){
    while (1) {
        log_info(logger, "Esperando mensajes de memoria");
		int cod_op = recibir_operacion(cpu_cliente_memoria);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(cpu_cliente_memoria);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case -1:
			log_error(logger, "La Memoria se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_memoria_cpu_sin_while(){
        log_info(logger, "Esperando mensajes de memoria");
		t_buffer* buffer;
		int cod_op = recibir_operacion(cpu_cliente_memoria);
		switch (cod_op) {
		case HANDSHAKE:
			buffer = recibir_buffer(cpu_cliente_memoria);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case PEDIR_INSTRUCCION: //CASE para recibir la instruccion de memoria
			buffer = recibir_buffer(cpu_cliente_memoria);
			char* instruccion = extraer_string_buffer(buffer,logger);
			//Libera la anterior instruccion, solicita espacio para la nueva y la almacena
			free(instruccion_a_decodificar); 
			instruccion_a_decodificar = malloc(strlen(instruccion)+1);
			strcpy(instruccion_a_decodificar,instruccion);
			free(instruccion);
			break; 
		case -1:
			log_error(logger, "La Memoria se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}


	  

