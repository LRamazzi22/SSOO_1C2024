#include <atender_mensajes.h>

void atender_memoria(){
    while (1) {
		log_info(logger, "Esperando mensajes de Memoria");
		int cod_op = recibir_operacion(kernel_cliente_memoria);
		switch (cod_op) {
		case HANDSHAKE:
			t_buffer* buffer = recibir_buffer(kernel_cliente_memoria);
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

int recibir_PC_memoria(){
	int cod_op = recibir_operacion(kernel_cliente_memoria);
	switch (cod_op) {
	case CREAR_PROCESO:
		t_buffer* buffer = recibir_buffer(kernel_cliente_memoria);
		int pc = extraer_int_buffer(buffer, logger);
		return pc;
		break;
	case -1:
		log_error(logger, "Se desconceto la Memoria");
		exit(EXIT_FAILURE);
	default:
		log_warning(logger,"La operacion no es la de crear proceso");
		return -1;
		break;
	}
}

void atender_cpu_dispatch(){
		log_info(logger, "Esperando mensajes de CPU Dispatch");
		int cod_op = recibir_operacion(kernel_cliente_dispatch);
		t_buffer* buffer;
		switch (cod_op) {
		case HANDSHAKE:
			buffer = recibir_buffer(kernel_cliente_dispatch);
			char* mensaje = extraer_string_buffer(buffer, logger);
			printf("Recibi un handshake de: %s, como cliente",mensaje);
			free(mensaje);
			break;
		case FINALIZAR_EXEC:
			buffer = recibir_buffer(kernel_cliente_dispatch);
			pcb* pcb_a_finalizar = proceso_en_ejecucion;
			recibir_contexto_de_ejecucion(buffer,pcb_a_finalizar);
			pcb_a_finalizar->estado_proceso = EXIT_PROCESS;
			queue_push(cola_exit,pcb_a_finalizar);
			// LIBERAR LO QUE CORRESPONDA 
			break;
		case ESPERAR_GEN:
			buffer = recibir_buffer(kernel_cliente_dispatch);
			pcb* pcb_del_proceso = proceso_en_ejecucion;
			recibir_contexto_de_ejecucion(buffer,pcb_del_proceso);
			char* interfaz = extraer_string_buffer(buffer,logger);
			char* tipo_interfaz = "Generica";
			int tiempo_espera = extraer_int_buffer(buffer,logger);
			nodo_de_diccionario_interfaz* nodo_de_interfaz = comprobrar_existencia_de_interfaz(pcb_del_proceso,interfaz,tipo_interfaz);
			if(nodo_de_interfaz != NULL){
				t_paquete* paquete = crear_paquete(ESPERAR_GEN);
				agregar_int_a_paquete(paquete,pcb_del_proceso->PID);
				agregar_int_a_paquete(paquete,tiempo_espera);
				enviar_paquete(paquete,nodo_de_interfaz->cliente);
				eliminar_paquete(paquete);
				estructura_para_atender_IO* pcb_y_cliente = malloc(sizeof(estructura_para_atender_IO));
				pcb_y_cliente ->cliente = nodo_de_interfaz->cliente;
				pcb_y_cliente ->pcb_proceso = pcb_del_proceso;
				pthread_t hilo_de_entradasalida;
				pthread_create(&hilo_de_entradasalida,NULL,(void*)atender_entradasalida_kernel,(void*)pcb_y_cliente);
				pthread_detach(hilo_de_entradasalida);
			}
			else{
				log_info(logger,"Interfaz Inexistente, proceso mandado a la cola de EXIT");
			}
			free(interfaz);
			break;
		case -1:
			log_error(logger, "El CPU Dispatch se desconecto");
			exit(EXIT_FAILURE);
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
nodo_de_diccionario_interfaz* comprobrar_existencia_de_interfaz(pcb* el_pcb, char* interfaz,char* tipo_interfaz){
	if(dictionary_has_key(diccionario_entrada_salida,interfaz)){
		nodo_de_diccionario_interfaz* nodo_de_interfaz = dictionary_get(diccionario_entrada_salida,interfaz);
		if(strcmp(nodo_de_interfaz->tipo_de_interfaz,tipo_interfaz)==0){
			el_pcb->estado_proceso = BLOCKED;
			queue_push(cola_blocked,el_pcb);
			return nodo_de_interfaz;
		}
	}
	el_pcb ->estado_proceso = EXIT_PROCESS;
	queue_push(cola_exit,el_pcb);
	return NULL;
}

void recibir_contexto_de_ejecucion(t_buffer* buffer,pcb* el_pcb) {
	*el_pcb->registros_cpu_en_pcb->PC = extraer_uint32_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->AX = extraer_uint8_buffer(buffer, logger);	
	*el_pcb->registros_cpu_en_pcb->BX = extraer_uint8_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->CX = extraer_uint8_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->DX = extraer_uint8_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->EAX = extraer_uint32_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->EBX = extraer_uint32_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->ECX = extraer_uint32_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->EDX = extraer_uint32_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->SI = extraer_uint32_buffer(buffer, logger);
	*el_pcb->registros_cpu_en_pcb->DI = extraer_uint32_buffer(buffer, logger);
}


void atender_cpu_interrupt(){
    while (1) {
		log_info(logger, "Esperando mensajes de CPU Interrupt");
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

void atender_entradasalida_kernel(void* pcb_cliente){
	estructura_para_atender_IO* pcb_y_cliente = pcb_cliente;
	bool continuar_while = true;
	while(continuar_while){
		int cod_op = recibir_operacion(pcb_y_cliente ->cliente);
		switch (cod_op) {
		case EXITO_IO:
			t_buffer* buffer = recibir_buffer(pcb_y_cliente ->cliente);
			int pid = extraer_int_buffer(buffer,logger);
			if(pid == pcb_y_cliente->pcb_proceso->PID){
				pcb_y_cliente->pcb_proceso->estado_proceso = READY;
				eliminar_pcb_cola(cola_blocked,pcb_y_cliente->pcb_proceso);
				queue_push(cola_blocked_ready,pcb_y_cliente->pcb_proceso);
				continuar_while = false;
			}

		case -1:
			log_info(logger, "Entrada/Salida se desconecto");
			pcb_y_cliente->pcb_proceso->estado_proceso = EXIT;
			eliminar_pcb_cola(cola_blocked,pcb_y_cliente->pcb_proceso);
			queue_push(cola_exit,pcb_y_cliente->pcb_proceso);
			continuar_while = false;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}

	}
	free(pcb_y_cliente);
	
	
}