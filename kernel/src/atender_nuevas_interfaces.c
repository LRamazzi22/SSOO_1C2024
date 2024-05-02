#include <atender_nuevas_interfaces.h>



void atender_nueva_interfaz(void* cliente_entradasalida){
    int* cliente = cliente_entradasalida;
    int cod_op = recibir_operacion(*cliente);
    if(cod_op == PRIMERA_CONEXION_IO){
        t_buffer* buffer = recibir_buffer(*cliente);
        char* nombre_interfaz = extraer_string_buffer(buffer,logger);
        char* tipo_interfaz = extraer_string_buffer(buffer,logger);
        nodo_de_diccionario_interfaz* nodo = malloc(sizeof(nodo_de_diccionario_interfaz));
        nodo ->tipo_de_interfaz = tipo_interfaz;
        nodo ->cliente = *cliente;

        pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
        dictionary_put(diccionario_entrada_salida,nombre_interfaz,nodo);
        pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

        nodo_de_diccionario_blocked* nuevo_nodo_blocked = malloc(sizeof(nodo_de_diccionario_blocked));

        nuevo_nodo_blocked ->cola_bloqueados = queue_create();

        pthread_mutex_lock(&mutex_para_diccionario_blocked);
        dictionary_put(diccionario_blocked,nombre_interfaz,nuevo_nodo_blocked);
        pthread_mutex_unlock(&mutex_para_diccionario_blocked);

        nombre_y_cliente* nombre_cliente= malloc(sizeof(nombre_y_cliente));
        nombre_cliente ->cliente = *cliente;
        nombre_cliente ->nombre = nombre_interfaz;
        pthread_t hilo_entradasalida_desconexion;
        pthread_create(&hilo_entradasalida_desconexion,NULL, (void*)atender_mensajes_interfaz,(void*)nombre_cliente);
        pthread_join(hilo_entradasalida_desconexion,NULL);

    }
    else{
        log_error(logger,"Se envio el mensaje equivocado");
        liberar_conexion(*cliente);
    }
    free(cliente);
}

void atender_mensajes_interfaz(void* nombre_interfaz_y_cliente){
    nombre_y_cliente* nombre_cliente = nombre_interfaz_y_cliente;
    bool liberar_while = false;
    while(!liberar_while){

        int cod_op = recibir_operacion(nombre_cliente ->cliente);
        switch (cod_op)
        {
        case PRIMERA_CONEXION_IO:
            break;
        case EXITO_IO:
        
            t_buffer* buffer = recibir_buffer(nombre_cliente ->cliente);
			int pid = extraer_int_buffer(buffer,logger);


			pthread_mutex_lock(&mutex_para_diccionario_blocked);
			nodo_de_diccionario_blocked* nodo_bloqueados = dictionary_get(diccionario_blocked, nombre_cliente ->nombre);
			pthread_mutex_unlock(&mutex_para_diccionario_blocked);

            pcb* un_pcb = buscar_proceso_en_cola(pid,nodo_bloqueados);

            if(un_pcb != NULL){
                pthread_mutex_lock(&(nodo_bloqueados ->mutex_para_cola));
                eliminar_pcb_cola(nodo_bloqueados -> cola_bloqueados, un_pcb);
                pthread_mutex_unlock(&(nodo_bloqueados ->mutex_para_cola));

                un_pcb ->estado_proceso = READY;

			    pthread_mutex_lock(&mutex_cola_ready);
			    queue_push(cola_ready,un_pcb);
			    pthread_mutex_unlock(&mutex_cola_ready);
                sem_post(&hay_proceso_en_ready);
            }
				
            break;

        case -1:
            pthread_mutex_lock(&mutex_para_eliminar_entradasalida);

            pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
            nodo_de_diccionario_interfaz* nodo = dictionary_remove(diccionario_entrada_salida,nombre_cliente ->nombre);
            pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);
            free(nodo ->tipo_de_interfaz);
            liberar_conexion(nodo ->cliente);
            free(nodo);

            pcb* el_pcb;

            pthread_mutex_lock(&mutex_para_diccionario_blocked);
            nodo_de_diccionario_blocked* nodo_de_bloqueados = dictionary_remove(diccionario_blocked,nombre_cliente ->nombre);
            pthread_mutex_unlock(&mutex_para_diccionario_blocked);

            int cantidad_procesos_bloqueados;

            pthread_mutex_lock(&(nodo_de_bloqueados->mutex_para_cola));
            cantidad_procesos_bloqueados = queue_size(nodo_de_bloqueados -> cola_bloqueados);
            pthread_mutex_unlock(&(nodo_de_bloqueados->mutex_para_cola));

            for(int i = 0; i<cantidad_procesos_bloqueados; i++){
                pthread_mutex_lock(&(nodo_de_bloqueados->mutex_para_cola));
                el_pcb = queue_pop(nodo_de_bloqueados -> cola_bloqueados);
                pthread_mutex_unlock(&(nodo_de_bloqueados->mutex_para_cola));

                el_pcb ->estado_proceso = EXIT;

                pthread_mutex_lock(&mutex_cola_exit);
                queue_push(cola_exit,el_pcb);
                pthread_mutex_unlock(&mutex_cola_exit);
            }
            queue_destroy(nodo_de_bloqueados ->cola_bloqueados);
            liberar_while = true;

            pthread_mutex_unlock(&mutex_para_eliminar_entradasalida);
            break;
    
        default:
            break;
        }
    }

}

pcb* buscar_proceso_en_cola(int pid, nodo_de_diccionario_blocked* nodo){
    int cantidad_procesos;
    pthread_mutex_lock(&(nodo ->mutex_para_cola));
    cantidad_procesos = queue_size(nodo ->cola_bloqueados);
    pthread_mutex_unlock(&(nodo ->mutex_para_cola));
    pcb* pcb_revisar;
    for(int i = 0; i<cantidad_procesos; i++){
        pthread_mutex_lock(&(nodo ->mutex_para_cola));
        pcb_revisar = list_get(nodo -> cola_bloqueados -> elements,i);
        pthread_mutex_unlock(&(nodo ->mutex_para_cola));
        if(pcb_revisar->PID == pid){
            return pcb_revisar;
        }
    }
    return NULL;

}

void atender_las_nuevas_interfaces(){
    int* cliente;
    while(1){
        cliente = malloc(sizeof(int));
        *cliente = esperar_cliente(kernel_server, logger, "Entrada Salida Conectado");
        pthread_t hilo_atender_entradasalida;
        pthread_create(&hilo_atender_entradasalida,NULL, (void*)atender_nueva_interfaz,(void*)cliente);
        pthread_detach(hilo_atender_entradasalida);
    }
    
}