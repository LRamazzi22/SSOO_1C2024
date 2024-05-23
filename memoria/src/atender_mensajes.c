#include <atender_mensajes.h>

void atender_cpu_memoria(){
	t_buffer* buffer;
	char* mensaje;
    while (1) {
		//log_info(logger, "Esperando mensajes de CPU");
		int cod_op = recibir_operacion(cpu_cliente);
		buffer = recibir_buffer(cpu_cliente);
		switch (cod_op) {
		case HANDSHAKE:
			mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case PROTOCOLO:
			int num = extraer_int_buffer(buffer, logger);
			mensaje = extraer_string_buffer(buffer, logger);
			printf("El numero es: %d \n", num);
			printf("%s", mensaje);
			free(mensaje);
			break;
		case PEDIR_INSTRUCCION:
			usleep(RETARDO_RESPUESTA * 1000);
			int pid = extraer_int_buffer(buffer,logger);
			int pc = extraer_int_buffer(buffer,logger);
			enviar_instruccion(pc,pid);
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

void enviar_instruccion(int pc,int pid){
	t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION);
	char* pid_clave = string_itoa(pid);

	pthread_mutex_lock(&mutex_para_diccionario_instrucciones);
	t_list* lista_instrucciones = dictionary_get(diccionario_de_instrucciones,pid_clave);
	pthread_mutex_unlock(&mutex_para_diccionario_instrucciones);

	char * instruccion = list_get(lista_instrucciones,pc);
	agregar_string_a_paquete(paquete, instruccion);
	enviar_paquete(paquete, cpu_cliente);
    eliminar_paquete(paquete);
}

void atender_kernel_memoria(){
    while (1) {
        log_info(logger, "Esperando mensajes de Kernel");
		int cod_op = recibir_operacion(kernel_cliente);
		t_buffer* buffer;
		switch (cod_op) {
		case HANDSHAKE:
			buffer = recibir_buffer(kernel_cliente);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case CREAR_PROCESO:
			buffer = recibir_buffer(kernel_cliente);
			int pid = extraer_int_buffer(buffer,logger);
			char* ruta_pseudocodigo = extraer_string_buffer(buffer,logger);
			int programCounter = leer_archivo(ruta_pseudocodigo,diccionario_de_instrucciones,pid);
			enviar_program_counter(programCounter);
			break;
		case ELIMINAR_PROCESO_MEMORIA:
			buffer = recibir_buffer(kernel_cliente);
			int pid2 = extraer_int_buffer(buffer,logger);
			char* pid_clave = string_itoa(pid2);

			pthread_mutex_lock(&mutex_para_diccionario_instrucciones);
			t_list* lista_instrucciones = dictionary_remove(diccionario_de_instrucciones,pid_clave);
			pthread_mutex_unlock(&mutex_para_diccionario_instrucciones);
			
			for(int i = 0; i<list_size(lista_instrucciones); i++){
				void* instruccion = list_get(lista_instrucciones,i);
				free(instruccion);
			}
			list_destroy(lista_instrucciones);
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

void enviar_program_counter(int pc){
	t_paquete* paquete = crear_paquete(CREAR_PROCESO);
	agregar_int_a_paquete(paquete,pc);
	enviar_paquete(paquete,kernel_cliente);
	eliminar_paquete(paquete);
}

void atender_entradasalida_memoria(void* cliente){
	int* cliente_entrada_salida = cliente;
	bool continuar_while = true;
    while (continuar_while) {
        log_info(logger, "Esperando mensajes de Entrada/Salida");
		int cod_op = recibir_operacion(*cliente_entrada_salida);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(*cliente_entrada_salida);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case -1:
			log_info(logger, "La Entrada/Salida se desconecto");
			continuar_while = false;
			break;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}