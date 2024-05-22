#include <atender_nuevas_interfaces.h>



void atender_nueva_interfaz(void* cliente_entradasalida){
    int* cliente_momentaneo = cliente_entradasalida;
    int* cliente = malloc(sizeof(int));
    * cliente = *cliente_momentaneo;
    int cod_op = recibir_operacion(*cliente);
    if(cod_op == PRIMERA_CONEXION_IO){
        t_buffer* buffer = recibir_buffer(*cliente);
        char* nombre_interfaz = extraer_string_buffer(buffer,logger);
        char* tipo_interfaz = extraer_string_buffer(buffer,logger);
        nodo_de_diccionario_interfaz* nodo = malloc(sizeof(nodo_de_diccionario_interfaz));
        nodo ->tipo_de_interfaz = tipo_interfaz;
        nodo ->cliente = cliente;
        sem_init(&(nodo ->se_puede_enviar_proceso),0,1);
        sem_init(&(nodo ->hay_proceso_en_bloqueados),0,0);
        sem_init(&(nodo ->detener_planificacion_enviar_peticion_IO),0,0);
        sem_init(&(nodo ->detener_planificacion_recibir_respuestas_IO),0,0);

        pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
        dictionary_put(diccionario_entrada_salida,nombre_interfaz,nodo);
        pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

        nodo_de_diccionario_blocked* nuevo_nodo_blocked = malloc(sizeof(nodo_de_diccionario_blocked));

        nuevo_nodo_blocked ->cola_bloqueados = queue_create();
        nuevo_nodo_blocked ->cola_Variables = queue_create();

        pthread_mutex_lock(&mutex_para_diccionario_blocked);
        dictionary_put(diccionario_blocked,nombre_interfaz,nuevo_nodo_blocked);
        pthread_mutex_unlock(&mutex_para_diccionario_blocked);

        nombre_y_cliente* nombre_cliente= malloc(sizeof(nombre_y_cliente));
        nombre_cliente ->cliente = cliente;
        nombre_cliente ->nombre = nombre_interfaz;
        pthread_t hilo_entradasalida_atender_mensajes;
        pthread_t hilo_entradasalida_enviar_proceso;
        pthread_create(&hilo_entradasalida_enviar_proceso,NULL, (void*)enviar_proceso_interfaz,(void*)nombre_cliente);
        pthread_detach(hilo_entradasalida_enviar_proceso);
        pthread_create(&hilo_entradasalida_atender_mensajes,NULL, (void*)atender_mensajes_interfaz,(void*)nombre_cliente);
        pthread_join(hilo_entradasalida_atender_mensajes,NULL);

    }
    else{
        log_error(logger,"Se envio el mensaje equivocado");
        liberar_conexion(*cliente);
    }
}

void atender_mensajes_interfaz(void* nombre_interfaz_y_cliente){
    nombre_y_cliente* nombre_cliente = nombre_interfaz_y_cliente;
    bool continuar_while = true;
    t_buffer* buffer;
    while(continuar_while){

        int cod_op = recibir_operacion(*nombre_cliente ->cliente);

        if(!permitir_planificacion){
            pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
            nodo_de_diccionario_interfaz* nodo_para_detener = dictionary_get(diccionario_entrada_salida,nombre_cliente ->nombre);
            pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);


            sem_wait(&(nodo_para_detener ->detener_planificacion_recibir_respuestas_IO));
        }

        switch (cod_op)
        {
        case HANDSHAKE:
            buffer = recibir_buffer(*nombre_cliente ->cliente);
			char* mensaje = extraer_string_buffer(buffer, logger);
			log_info(logger,mensaje);
			free(mensaje);
			break;  
        case PRIMERA_CONEXION_IO:
            break;
        case EXITO_IO:
            buffer = recibir_buffer(*nombre_cliente ->cliente);
			int pid = extraer_int_buffer(buffer,logger);


			pthread_mutex_lock(&mutex_para_diccionario_blocked);
			nodo_de_diccionario_blocked* nodo_bloqueados = dictionary_get(diccionario_blocked, nombre_cliente ->nombre);
			pthread_mutex_unlock(&mutex_para_diccionario_blocked);

            pcb* un_pcb = buscar_proceso_en_cola(pid,nodo_bloqueados);

            if(un_pcb != NULL){
                pthread_mutex_lock(&(nodo_bloqueados ->mutex_para_cola_bloqueados));
                eliminar_pcb_cola(nodo_bloqueados -> cola_bloqueados, un_pcb);
                pthread_mutex_unlock(&(nodo_bloqueados ->mutex_para_cola_bloqueados));

                un_pcb ->estado_proceso = READY;
                log_info(logger_obligatorio,"PID: %d - Estado Anterior: BLOCKED - Estado Actual: READY", un_pcb ->PID);

			    pthread_mutex_lock(&mutex_cola_ready);
			    queue_push(cola_ready,un_pcb);
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
			
            pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
            nodo_de_diccionario_interfaz* nodo_interfaz = dictionary_get(diccionario_entrada_salida,nombre_cliente ->nombre);
            pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

            sem_post(&(nodo_interfaz ->se_puede_enviar_proceso));
            break;

        case -1:
            pthread_mutex_lock(&mutex_para_eliminar_entradasalida);

            pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
            nodo_de_diccionario_interfaz* nodo = dictionary_remove(diccionario_entrada_salida,nombre_cliente ->nombre);
            pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);
            free(nodo ->tipo_de_interfaz);
            liberar_conexion(*nodo ->cliente);
            free(nodo ->cliente);
            sem_destroy(&(nodo ->hay_proceso_en_bloqueados));
            sem_destroy(&(nodo ->se_puede_enviar_proceso));
            pthread_mutex_destroy(&(nodo ->mutex_interfaz_siendo_usada));
            free(nodo);

            pcb* el_pcb;

            pthread_mutex_lock(&mutex_para_diccionario_blocked);
            nodo_de_diccionario_blocked* nodo_de_bloqueados = dictionary_remove(diccionario_blocked,nombre_cliente ->nombre);
            pthread_mutex_unlock(&mutex_para_diccionario_blocked);

            int cantidad_procesos_bloqueados;

            pthread_mutex_lock(&(nodo_de_bloqueados->mutex_para_cola_bloqueados));
            cantidad_procesos_bloqueados = queue_size(nodo_de_bloqueados -> cola_bloqueados);
            pthread_mutex_unlock(&(nodo_de_bloqueados->mutex_para_cola_bloqueados));

            for(int i = 0; i<cantidad_procesos_bloqueados; i++){
                pthread_mutex_lock(&(nodo_de_bloqueados->mutex_para_cola_bloqueados));
                el_pcb = queue_pop(nodo_de_bloqueados -> cola_bloqueados);
                pthread_mutex_unlock(&(nodo_de_bloqueados->mutex_para_cola_bloqueados));

                pthread_mutex_lock(&(nodo_de_bloqueados ->mutex_para_cola_variables));
                void* variable_a_borrar = queue_pop(nodo_de_bloqueados ->cola_Variables);
                free(variable_a_borrar);
                pthread_mutex_unlock(&(nodo_de_bloqueados ->mutex_para_cola_variables));

                el_pcb ->razon_salida = INTERFAZ_INVALIDA;

                mandar_a_exit(el_pcb);
                log_info(logger_obligatorio, "PID: %d - Estado Anterior: BLOCKED - Estado Actual: EXIT", el_pcb->PID);

            }
            queue_destroy(nodo_de_bloqueados ->cola_bloqueados);
            queue_destroy(nodo_de_bloqueados ->cola_Variables);
            pthread_mutex_destroy(&(nodo_de_bloqueados ->mutex_para_cola_bloqueados));
            pthread_mutex_destroy(&(nodo_de_bloqueados ->mutex_para_cola_variables));
            free(nodo_de_bloqueados);
            continuar_while = false;

            pthread_mutex_unlock(&mutex_para_eliminar_entradasalida);
            break;
    
        default:
            log_info(logger,"Nada paso");
            break;
        }
    }

}

void enviar_proceso_interfaz(void* nombre_interfaz_y_cliente){
    
    nombre_y_cliente* nombre_cliente = nombre_interfaz_y_cliente;
    bool continuar_while = true;

    pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
    nodo_de_diccionario_interfaz* nodo_interfaz = dictionary_get(diccionario_entrada_salida,nombre_cliente ->nombre);
    pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);
   

    pthread_mutex_lock(&mutex_para_diccionario_blocked);
    nodo_de_diccionario_blocked* nodo_blocked = dictionary_get(diccionario_blocked,nombre_cliente->nombre);
    pthread_mutex_unlock(&mutex_para_diccionario_blocked);
    
    
    while(continuar_while){
        
        sem_wait(&(nodo_interfaz ->hay_proceso_en_bloqueados));
        sem_wait(&(nodo_interfaz ->se_puede_enviar_proceso));

        if(!permitir_planificacion){
            
            sem_wait(&(nodo_interfaz ->detener_planificacion_enviar_peticion_IO));
        }
        

        pthread_mutex_lock(&(nodo_blocked ->mutex_para_cola_bloqueados));
        pcb* pcb_a_enviar = queue_peek(nodo_blocked ->cola_bloqueados);
        pthread_mutex_unlock(&(nodo_blocked ->mutex_para_cola_bloqueados));


        if(strcmp(nodo_interfaz ->tipo_de_interfaz, "Generica")==0){
            pthread_mutex_lock(&(nodo_blocked ->mutex_para_cola_variables));
            int* tiempo_espera = queue_pop(nodo_blocked ->cola_Variables);
            pthread_mutex_unlock(&(nodo_blocked ->mutex_para_cola_variables));

            t_paquete* paquete = crear_paquete(ESPERAR_GEN);
	        agregar_int_a_paquete(paquete,pcb_a_enviar ->PID);
	        agregar_int_a_paquete(paquete,*tiempo_espera);
	        enviar_paquete(paquete,*nodo_interfaz ->cliente);
	        eliminar_paquete(paquete);
            free(tiempo_espera);
        }
        else if(strcmp(nodo_interfaz ->tipo_de_interfaz, "stdin")==0){
            
        }
        else if(strcmp(nodo_interfaz ->tipo_de_interfaz, "stdout")==0){
            
        }
        else if(strcmp(nodo_interfaz ->tipo_de_interfaz, "dialfs")==0){
            
        }

        
    }
}

pcb* buscar_proceso_en_cola(int pid, nodo_de_diccionario_blocked* nodo){
    int cantidad_procesos;
    pthread_mutex_lock(&(nodo ->mutex_para_cola_bloqueados));
    cantidad_procesos = queue_size(nodo ->cola_bloqueados);
    pthread_mutex_unlock(&(nodo ->mutex_para_cola_bloqueados));
    pcb* pcb_revisar;
    for(int i = 0; i<cantidad_procesos; i++){
        pthread_mutex_lock(&(nodo ->mutex_para_cola_bloqueados));
        pcb_revisar = list_get(nodo -> cola_bloqueados -> elements,i);
        pthread_mutex_unlock(&(nodo ->mutex_para_cola_bloqueados));
        if(pcb_revisar->PID == pid){
            return pcb_revisar;
        }
    }
    return NULL;

}

void atender_las_nuevas_interfaces(){

    while(1){
        int cliente = esperar_cliente(kernel_server, logger, "Entrada Salida Conectado");
        pthread_t hilo_atender_entradasalida;
        pthread_create(&hilo_atender_entradasalida,NULL, (void*)atender_nueva_interfaz,(void*)&cliente);
        pthread_detach(hilo_atender_entradasalida);
    }
    
}