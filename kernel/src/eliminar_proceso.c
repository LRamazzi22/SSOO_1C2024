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
    //HAY QUE HACER QUE LE DEVUELVA LAS INSTANCIAS A LOS RECURSOS ANTES DE ELIMINAR LA LISTA
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