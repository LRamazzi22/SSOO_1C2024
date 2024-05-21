#include <eliminar_proceso.h>

void mandar_a_exit(pcb* pcb_a_finalizar){
    pcb_a_finalizar->estado_proceso = EXIT_PROCESS;

	pthread_mutex_lock(&mutex_cola_exit);
	queue_push(cola_exit,pcb_a_finalizar);
	pthread_mutex_unlock(&mutex_cola_exit);
	sem_post(&hay_proceso_en_exit);

}

void eliminar_el_proceso(pcb* un_pcb){
    t_paquete* paquete = crear_paquete(ELIMINAR_PROCESO_MEMORIA);
    agregar_int_a_paquete(paquete,un_pcb ->PID);
    enviar_paquete(paquete,kernel_cliente_memoria);
    eliminar_paquete(paquete);
    borrar_registros_pcb(un_pcb);
    /*
    for(int i = 0; i<list_size(un_pcb ->lista_recursos_tomados); i++){
        char* un_recurso = list_remove(un_pcb ->lista_recursos_tomados, i);
        printf("%s",un_recurso);

        pthread_mutex_lock(&mutex_para_diccionario_recursos);
        nodo_recursos* nodo_del_recurso = dictionary_get(diccionario_recursos,un_recurso);
        pthread_mutex_unlock(&mutex_para_diccionario_recursos);

        pthread_mutex_lock(&(nodo_del_recurso ->mutex_del_recurso));
		nodo_del_recurso ->instancias ++;

	    if(nodo_del_recurso ->instancias <= 0){
			pcb* pcb_a_desbloquear = queue_pop(nodo_del_recurso ->cola_bloqueados_recurso);
			pcb_a_desbloquear->estado_proceso = READY;
			log_info(logger_obligatorio, "PID: %d - Estado Anterior: BLOCKED - Estado Actual: READY", pcb_a_desbloquear->PID);
			pcb_a_desbloquear ->quantum_restante = QUANTUM;


			pthread_mutex_lock(&mutex_cola_ready);
			queue_push(cola_ready,pcb_a_desbloquear);
			char* lista = malloc(1);
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
        pthread_mutex_unlock(&(nodo_del_recurso ->mutex_del_recurso));
    }
    */
    list_destroy(un_pcb ->lista_recursos_tomados);
    switch(un_pcb ->razon_salida){
        case EXITO:
            log_info(logger_obligatorio, "Finaliza el proceso %d - Motivo: SUCCESS",un_pcb->PID);
            break;
        case RECURSO_INVALIDO:
            log_info(logger_obligatorio, "Finaliza el proceso %d - Motivo: INVALID_RESOURCE",un_pcb->PID);
            break;
        case INTERFAZ_INVALIDA:
            log_info(logger_obligatorio, "Finaliza el proceso %d - Motivo: INVALID_INTERFACE",un_pcb->PID);
            break;
        case FUERA_DE_MEMORIA:
            log_info(logger_obligatorio, "Finaliza el proceso %d - Motivo: OUT_OF_MEMORY",un_pcb->PID);
            break;
        case FINALIZADO_POR_USUARIO:
            log_info(logger_obligatorio, "Finaliza el proceso %d - Motivo: INTERRUPTED_BY_USER",un_pcb->PID);
            break;
    }
    free(un_pcb);
    if(espera_grado_multi > 0){
        espera_grado_multi--;
    }
    else{
        sem_post(&multiprogramacion_permite_proceso_en_ready);
    }
    
}
void borrar_registros_pcb(pcb* un_pcb){
    free(un_pcb->registros_cpu_en_pcb->AX);
    free(un_pcb->registros_cpu_en_pcb->BX);
    free(un_pcb->registros_cpu_en_pcb->CX);
    free(un_pcb->registros_cpu_en_pcb->DX);
    free(un_pcb->registros_cpu_en_pcb->EAX);
    free(un_pcb->registros_cpu_en_pcb->EBX);
    free(un_pcb->registros_cpu_en_pcb->ECX);
    free(un_pcb->registros_cpu_en_pcb->EDX);
    free(un_pcb->registros_cpu_en_pcb->SI);
    free(un_pcb->registros_cpu_en_pcb->DI);
    free(un_pcb->registros_cpu_en_pcb->PC);
}