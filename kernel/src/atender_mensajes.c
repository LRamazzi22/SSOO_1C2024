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

			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pcb* pcb_a_finalizar = proceso_en_ejecucion;
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

			recibir_contexto_de_ejecucion(buffer,pcb_a_finalizar);
			pcb_a_finalizar->estado_proceso = EXIT_PROCESS;

			pthread_mutex_lock(&mutex_cola_exit);
			queue_push(cola_exit,pcb_a_finalizar);
			pthread_mutex_unlock(&mutex_cola_exit);
			sem_post(&hay_proceso_en_exit);

			break;
		case ESPERAR_GEN:
			buffer = recibir_buffer(kernel_cliente_dispatch);

			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pcb* pcb_del_proceso = proceso_en_ejecucion;
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

			recibir_contexto_de_ejecucion(buffer,pcb_del_proceso);

			char* interfaz = extraer_string_buffer(buffer,logger);
			char* tipo_interfaz = "Generica";
			int tiempo_espera = extraer_int_buffer(buffer,logger);

			pthread_mutex_lock(&mutex_para_eliminar_entradasalida);
			nodo_de_diccionario_interfaz* nodo_de_interfaz = comprobrar_existencia_de_interfaz(pcb_del_proceso,interfaz,tipo_interfaz);

			if(nodo_de_interfaz != NULL){
				t_paquete* paquete = crear_paquete(ESPERAR_GEN);
				agregar_int_a_paquete(paquete,pcb_del_proceso->PID);
				agregar_int_a_paquete(paquete,tiempo_espera);
				enviar_paquete(paquete,nodo_de_interfaz->cliente);
				eliminar_paquete(paquete);

			}
			else{
				log_info(logger,"Interfaz Inexistente, proceso mandado a la cola de EXIT");
			}
			pthread_mutex_unlock(&mutex_para_eliminar_entradasalida);
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
	bool tiene_la_interfaz;

	pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
	tiene_la_interfaz = dictionary_has_key(diccionario_entrada_salida,interfaz);
	pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

	if(tiene_la_interfaz){

		pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
		nodo_de_diccionario_interfaz* nodo_de_interfaz = dictionary_get(diccionario_entrada_salida,interfaz);
		pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

		if(nodo_de_interfaz != NULL && strcmp(nodo_de_interfaz->tipo_de_interfaz,tipo_interfaz)==0){
			el_pcb->estado_proceso = BLOCKED;

			pthread_mutex_lock(&mutex_para_diccionario_blocked);
			nodo_de_diccionario_blocked* nodo_bloqueados = dictionary_get(diccionario_blocked,interfaz);
			pthread_mutex_unlock(&mutex_para_diccionario_blocked);

			pthread_mutex_lock(&(nodo_bloqueados ->mutex_para_cola));
			queue_push(nodo_bloqueados ->cola_bloqueados,el_pcb);
			pthread_mutex_unlock(&(nodo_bloqueados ->mutex_para_cola));
			return nodo_de_interfaz;
		}
	}
	el_pcb ->estado_proceso = EXIT_PROCESS;
	pthread_mutex_lock(&mutex_cola_exit);
	queue_push(cola_exit,el_pcb);
	pthread_mutex_unlock(&mutex_cola_exit);
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


