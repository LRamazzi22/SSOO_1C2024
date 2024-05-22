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

		if(strcmp(ALGORITMO_PLANIFICACION,"rr")==0 || strcmp(ALGORITMO_PLANIFICACION,"vrr")==0){
			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pthread_cancel(proceso_en_ejecucion ->hilo_quantum);
			temporal_stop(proceso_en_ejecucion ->tiempo_en_ejecucion);
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);
		}


		if(!permitir_planificacion){
            sem_wait(&detener_planificacion_salida_cpu);
        }

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

			if(pcb_a_finalizar ->PID == pid_a_eliminar){ //Si el proceso que salio del cpu es al que se le mando la interrupcion de eliminarlo
				pcb_a_finalizar ->razon_salida = FINALIZADO_POR_USUARIO;
				mandar_a_exit(pcb_a_finalizar);
				log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: EXIT", pcb_a_finalizar ->PID);
				pid_a_eliminar = -1;
				break;
			}

			pcb_a_finalizar ->razon_salida = EXITO;
			mandar_a_exit(pcb_a_finalizar);
			log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: EXIT", pcb_a_finalizar ->PID);
			 
			break;
		case ESPERAR_GEN:
			buffer = recibir_buffer(kernel_cliente_dispatch);

			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pcb* pcb_del_proceso = proceso_en_ejecucion;
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

			recibir_contexto_de_ejecucion(buffer,pcb_del_proceso);


			char* interfaz = extraer_string_buffer(buffer,logger);
			char* tipo_interfaz = "Generica";
			int* tiempo_espera = malloc(sizeof(int));
			*tiempo_espera = extraer_int_buffer(buffer,logger);

			if(pcb_del_proceso ->PID == pid_a_eliminar){ //Si el proceso que salio del cpu es al que se le mando la interrupcion de eliminarlo
				pcb_del_proceso ->razon_salida = FINALIZADO_POR_USUARIO;
				mandar_a_exit(pcb_del_proceso);
				pid_a_eliminar = -1;
				free(interfaz);
				free(tiempo_espera);
				break;
			}

			pthread_mutex_lock(&mutex_para_eliminar_entradasalida);
			nodo_de_diccionario_interfaz* nodo_de_interfaz = comprobrar_existencia_de_interfaz(pcb_del_proceso,interfaz,tipo_interfaz);

			if(nodo_de_interfaz != NULL){
				pthread_mutex_lock(&mutex_para_diccionario_blocked);
				nodo_de_diccionario_blocked* nodo_bloqueados = dictionary_get(diccionario_blocked,interfaz);
				pthread_mutex_unlock(&mutex_para_diccionario_blocked);

				pthread_mutex_lock(&(nodo_bloqueados ->mutex_para_cola_variables));
				queue_push(nodo_bloqueados ->cola_Variables,tiempo_espera);
				pthread_mutex_unlock(&(nodo_bloqueados ->mutex_para_cola_variables));

				sem_post(&(nodo_de_interfaz ->hay_proceso_en_bloqueados));

			}
			else{
				log_info(logger,"Interfaz Inexistente, proceso mandado a la cola de EXIT");
			}
			pthread_mutex_unlock(&mutex_para_eliminar_entradasalida);
			free(interfaz);
			break;
		case WAIT_CODE:
			buffer = recibir_buffer(kernel_cliente_dispatch);

			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pcb* pcb_a_esperar = proceso_en_ejecucion;
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

			recibir_contexto_de_ejecucion(buffer,pcb_a_esperar);

			char* recurso = extraer_string_buffer(buffer,logger);
			printf("%s h",recurso);

			if(strcmp(recurso, RECURSOS[0])){
				printf("Si");
			}

			if(pcb_a_esperar ->PID == pid_a_eliminar){ //Si el proceso que salio del cpu es al que se le mando la interrupcion de eliminarlo
				pcb_a_esperar ->razon_salida = FINALIZADO_POR_USUARIO;
				mandar_a_exit(pcb_a_esperar);
				pid_a_eliminar = -1;
				free(recurso);
				break;
			}

			bool existe_recurso;

			pthread_mutex_lock(&mutex_para_diccionario_recursos);
			existe_recurso = dictionary_has_key(diccionario_recursos,recurso);
			pthread_mutex_unlock(&mutex_para_diccionario_recursos);

			if(existe_recurso){
				char* recurso_lista = strdup(recurso);
				list_add(pcb_a_esperar ->lista_recursos_tomados,recurso_lista);
				pthread_mutex_lock(&mutex_para_diccionario_recursos);
				nodo_recursos* nodo_del_recurso = dictionary_get(diccionario_recursos,recurso);
				pthread_mutex_unlock(&mutex_para_diccionario_recursos);

				pthread_mutex_lock(&(nodo_del_recurso ->mutex_del_recurso));
				nodo_del_recurso ->instancias = nodo_del_recurso ->instancias -1;
				

				if(nodo_del_recurso -> instancias < 0){
					pcb_a_esperar ->estado_proceso = BLOCKED;
					temporal_destroy(pcb_a_esperar ->tiempo_en_ejecucion);
					log_info(logger_obligatorio,"PID: %d - Estado Anterior: EXECUTE - Estado Actual: BLOCKED", pcb_a_esperar->PID);
					log_info(logger_obligatorio, "PID: %d - Bloqueado por: %s",pcb_a_esperar ->PID, recurso);
					queue_push(nodo_del_recurso ->cola_bloqueados_recurso,pcb_a_esperar);
				}
				else if(strcmp(ALGORITMO_PLANIFICACION,"rr")==0 || strcmp(ALGORITMO_PLANIFICACION,"vrr")==0){ //ROUND ROBIN O VIRTUAL ROUND ROBIN
					int64_t tiempo_ejecutado = temporal_gettime(pcb_a_esperar ->tiempo_en_ejecucion);
					temporal_destroy(pcb_a_esperar ->tiempo_en_ejecucion);
					if(tiempo_ejecutado < pcb_a_esperar ->quantum_restante){
						pcb_a_esperar ->quantum_restante = pcb_a_esperar ->quantum_restante - tiempo_ejecutado;

						t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    					agregar_int_a_paquete(paquete_pcb_a_enviar,pcb_a_esperar->PID);
    					serializar_registros_procesador(paquete_pcb_a_enviar, pcb_a_esperar->registros_cpu_en_pcb);
    
    					enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    					eliminar_paquete(paquete_pcb_a_enviar);
						pcb_a_esperar ->tiempo_en_ejecucion = temporal_create();
    					pthread_create(&(pcb_a_esperar ->hilo_quantum),NULL,(void*)esperar_quantum,(void*)pcb_a_esperar);
    					pthread_detach(pcb_a_esperar ->hilo_quantum);
						atender_cpu_dispatch();
					}
					else{
						log_info(logger_obligatorio,"PID: %d - Desalojado por fin de Quantum",pcb_a_esperar ->PID);
						pcb_a_esperar->estado_proceso = READY;
						log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: READY", pcb_a_esperar->PID);
						pcb_a_esperar ->quantum_restante = QUANTUM;


						pthread_mutex_lock(&mutex_cola_ready);
						queue_push(cola_ready,pcb_a_esperar);
						char* lista = malloc(3);
        				strcpy(lista,"[");
        				for(int i = 0; i < queue_size(cola_ready); i++){
            				pcb* un_pcb = list_get(cola_ready ->elements,i);
            				char* pid = string_itoa(un_pcb ->PID);
            				string_append(&lista, pid);
            				if(i != (queue_size(cola_ready)-1)){
                				string_append(&lista, ", ");
            				}
            
        				}
        				string_append(&lista, "]");
        				log_info(logger_obligatorio, "Cola Ready %s", lista);
        				free(lista);
						pthread_mutex_unlock(&mutex_cola_ready);

						sem_post(&hay_proceso_en_ready);
					}
				}
				else{ //ES FIFO
					t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    				agregar_int_a_paquete(paquete_pcb_a_enviar,pcb_a_esperar->PID);
    				serializar_registros_procesador(paquete_pcb_a_enviar, pcb_a_esperar->registros_cpu_en_pcb);
    
    				enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    				eliminar_paquete(paquete_pcb_a_enviar);
					atender_cpu_dispatch();
				}
			pthread_mutex_unlock(&(nodo_del_recurso ->mutex_del_recurso));
			}
			else{
				pcb_a_esperar ->razon_salida = RECURSO_INVALIDO;
				mandar_a_exit(pcb_a_esperar);
				
			}
			free(recurso);


			break;
		case SIGNAL_CODE:
			buffer = recibir_buffer(kernel_cliente_dispatch);

			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pcb* pcb_a_senial = proceso_en_ejecucion;
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

			recibir_contexto_de_ejecucion(buffer,pcb_a_senial);

			char* recurso_signal = extraer_string_buffer(buffer,logger);

			if(pcb_a_senial ->PID == pid_a_eliminar){ //Si el proceso que salio del cpu es al que se le mando la interrupcion de eliminarlo
				pcb_a_senial ->razon_salida = FINALIZADO_POR_USUARIO;
				mandar_a_exit(pcb_a_senial);
				pid_a_eliminar = -1;
				free(recurso_signal);
				break;
			}

			bool existe_recurso_signal;

			pthread_mutex_lock(&mutex_para_diccionario_recursos);
			existe_recurso_signal = dictionary_has_key(diccionario_recursos,recurso_signal);
			pthread_mutex_unlock(&mutex_para_diccionario_recursos);

			if(existe_recurso_signal){
				pthread_mutex_lock(&mutex_para_diccionario_recursos);
				nodo_recursos* nodo_del_recurso = dictionary_get(diccionario_recursos,recurso_signal);
				pthread_mutex_unlock(&mutex_para_diccionario_recursos);

				for (int i = 0; i<list_size(pcb_a_senial ->lista_recursos_tomados); i++){
					char* recurso_lista = list_get(pcb_a_senial ->lista_recursos_tomados,i);
					if(strcmp(recurso_lista,recurso_signal)==0){
						list_remove(pcb_a_senial ->lista_recursos_tomados,i);
						break;
					}
				}

				pthread_mutex_lock(&(nodo_del_recurso ->mutex_del_recurso));
				nodo_del_recurso ->instancias ++;

				if(nodo_del_recurso ->instancias <= 0){
					pcb* pcb_a_desbloquear = queue_pop(nodo_del_recurso ->cola_bloqueados_recurso);
					pcb_a_desbloquear->estado_proceso = READY;
					log_info(logger_obligatorio, "PID: %d - Estado Anterior: BLOCKED - Estado Actual: READY", pcb_a_desbloquear->PID);
					pcb_a_desbloquear ->quantum_restante = QUANTUM;


					pthread_mutex_lock(&mutex_cola_ready);
					queue_push(cola_ready,pcb_a_desbloquear);
					char* lista = malloc(3);
        			strcpy(lista,"[");
        			for(int i = 0; i < queue_size(cola_ready); i++){
            			pcb* un_pcb = list_get(cola_ready ->elements,i);
            			char* pid = string_itoa(un_pcb ->PID);
            			string_append(&lista, pid);
            			if(i != (queue_size(cola_ready)-1)){
                			string_append(&lista, ", ");
            			}
            
        			}
        			string_append(&lista, "]");
        			log_info(logger_obligatorio, "Cola Ready %s", lista);
        			free(lista);
					pthread_mutex_unlock(&mutex_cola_ready);

					sem_post(&hay_proceso_en_ready);
				}
				if(strcmp(ALGORITMO_PLANIFICACION,"rr")==0 || strcmp(ALGORITMO_PLANIFICACION,"vrr")==0){ //ROUND ROBIN O VIRTUAL ROUND ROBIN
					int64_t tiempo_ejecutado = temporal_gettime(pcb_a_senial ->tiempo_en_ejecucion);
					temporal_destroy(pcb_a_senial ->tiempo_en_ejecucion);
					if(tiempo_ejecutado < pcb_a_senial ->quantum_restante){
						pcb_a_senial ->quantum_restante = pcb_a_senial ->quantum_restante - tiempo_ejecutado;

						t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    					agregar_int_a_paquete(paquete_pcb_a_enviar,pcb_a_senial->PID);
    					serializar_registros_procesador(paquete_pcb_a_enviar, pcb_a_senial->registros_cpu_en_pcb);
    
    					enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    					eliminar_paquete(paquete_pcb_a_enviar);
						pcb_a_senial ->tiempo_en_ejecucion = temporal_create();
    					pthread_create(&(pcb_a_senial ->hilo_quantum),NULL,(void*)esperar_quantum,(void*)pcb_a_senial);
    					pthread_detach(pcb_a_senial ->hilo_quantum);
						atender_cpu_dispatch();
					}
					else{
						log_info(logger_obligatorio,"PID: %d - Desalojado por fin de Quantum",pcb_a_senial ->PID);
						pcb_a_senial->estado_proceso = READY;
						log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: READY", pcb_a_senial->PID);
						pcb_a_senial ->quantum_restante = QUANTUM;


						pthread_mutex_lock(&mutex_cola_ready);
						queue_push(cola_ready,pcb_a_senial);
						char* lista = malloc(3);
        				strcpy(lista,"[");
        				for(int i = 0; i < queue_size(cola_ready); i++){
            				pcb* un_pcb = list_get(cola_ready ->elements,i);
            				char* pid = string_itoa(un_pcb ->PID);
            				string_append(&lista, pid);
            				if(i != (queue_size(cola_ready)-1)){
                				string_append(&lista, ", ");
            				}
            
        				}
        				string_append(&lista, "]");
        				log_info(logger_obligatorio, "Cola Ready %s", lista);
        				free(lista);
						pthread_mutex_unlock(&mutex_cola_ready);

						sem_post(&hay_proceso_en_ready);
					}
				}
				else{ //ES FIFO
					t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    				agregar_int_a_paquete(paquete_pcb_a_enviar,pcb_a_senial->PID);
    				serializar_registros_procesador(paquete_pcb_a_enviar, pcb_a_senial->registros_cpu_en_pcb);
    
    				enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    				eliminar_paquete(paquete_pcb_a_enviar);
    				pcb_a_senial ->tiempo_en_ejecucion = temporal_create();
					atender_cpu_dispatch();
				}

			pthread_mutex_unlock(&(nodo_del_recurso ->mutex_del_recurso));
			}
			else{
				pcb_a_senial -> razon_salida = RECURSO_INVALIDO;
				mandar_a_exit(pcb_a_senial);
			}
			free(recurso_signal);
			break;
		case INTERRUPCION:
			buffer = recibir_buffer(kernel_cliente_dispatch);

			pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
			pcb* pcb_a_guardar = proceso_en_ejecucion;
			pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);


			recibir_contexto_de_ejecucion(buffer,pcb_a_guardar);

			if(pcb_a_guardar ->PID == pid_a_eliminar){ //Si el proceso que salio del cpu es al que se le mando la interrupcion de eliminarlo
				pcb_a_guardar ->razon_salida = FINALIZADO_POR_USUARIO;
				mandar_a_exit(pcb_a_guardar);
				log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: EXIT", pcb_a_guardar ->PID);
				pid_a_eliminar = -1;
				break;
			}
			log_info(logger_obligatorio,"PID: %d - Desalojado por fin de Quantum",pcb_a_guardar ->PID);
			pcb_a_guardar->estado_proceso = READY;
			log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: READY", pcb_a_guardar->PID);


			pthread_mutex_lock(&mutex_cola_ready);

			int quantum_restante_actual = QUANTUM - temporal_gettime(pcb_a_guardar->tiempo_en_ejecucion);
			
			if (ALGORITMO_PLANIFICACION = "vrr" && quantum_restante_actual > 0){
			// Mover el proceso a la cola auxiliar, SOLO PARA VRR
			pcb_a_guardar->quantum_restante = quantum_restante_actual;
        	pthread_mutex_lock(&mutex_cola_auxiliar);
        	queue_push(cola_auxiliar, pcb_a_guardar);
        	pthread_mutex_unlock(&mutex_cola_auxiliar);
        	sem_post(&hay_proceso_en_auxiliar);
			
			//Loggear la cola de listos contemplando los que están en Auxiliar
			
			} else 
			{

			queue_push(cola_ready,pcb_a_guardar);
			char* lista = malloc(3);
        	strcpy(lista,"[");
        	for(int i = 0; i < queue_size(cola_ready); i++){
            	pcb* un_pcb = list_get(cola_ready ->elements,i);
            	char* pid = string_itoa(un_pcb ->PID);
            	string_append(&lista, pid);
            	if(i != (queue_size(cola_ready)-1)){
                	string_append(&lista, ", ");
            	}
            
        	}
        	string_append(&lista, "]");
        	log_info(logger_obligatorio, "Cola Ready %s", lista);
        	free(lista);
			pthread_mutex_unlock(&mutex_cola_ready);

			sem_post(&hay_proceso_en_ready);
			}

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
			log_info(logger_obligatorio,"PID: %d - Estado Anterior: EXECUTE - Estado Actual: BLOCKED", el_pcb->PID);
			log_info(logger_obligatorio, "PID: %d - Bloqueado por: %s",el_pcb ->PID, interfaz);

			pthread_mutex_lock(&mutex_para_diccionario_blocked);
			nodo_de_diccionario_blocked* nodo_bloqueados = dictionary_get(diccionario_blocked,interfaz);
			pthread_mutex_unlock(&mutex_para_diccionario_blocked);

			pthread_mutex_lock(&(nodo_bloqueados ->mutex_para_cola_bloqueados));
			queue_push(nodo_bloqueados ->cola_bloqueados,el_pcb);
			pthread_mutex_unlock(&(nodo_bloqueados ->mutex_para_cola_bloqueados));
			return nodo_de_interfaz;
		}
	}
	el_pcb ->razon_salida = INTERFAZ_INVALIDA;

	mandar_a_exit(el_pcb);
	log_info(logger_obligatorio, "PID: %d - Estado Anterior: EXECUTE - Estado Actual: EXIT", el_pcb ->PID);

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


