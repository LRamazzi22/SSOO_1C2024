#include <planificador_largo_plazo.h>

void iniciar_planificacion_largo_plazo(){
    pthread_t hilo_new_to_ready;
    pthread_create(&hilo_new_to_ready,NULL,(void*)planificador_new_to_ready,NULL);
    pthread_detach(hilo_new_to_ready);

    pthread_t hilo_exit;
    pthread_create(&hilo_exit,NULL,(void*)planificador_exit,NULL);
    pthread_detach(hilo_exit);
}

void planificador_new_to_ready(){
    while(true){
        sem_wait(&multiprogramacion_permite_proceso_en_ready);
        sem_wait(&hay_proceso_en_new);


        pcb* un_pcb;
        pthread_mutex_lock(&mutex_cola_new);
        un_pcb = queue_pop(cola_new);
        pthread_mutex_unlock(&mutex_cola_new);

        un_pcb ->estado_proceso = READY;

        pthread_mutex_lock(&mutex_cola_ready);
        queue_push(cola_ready,un_pcb);
        pthread_mutex_unlock(&mutex_cola_ready);

        sem_post(&hay_proceso_en_ready);
    }
}

void planificador_exit(){
   while(true){
    sem_wait(&hay_proceso_en_exit);
    pcb* un_pcb;
    
    pthread_mutex_lock(&mutex_cola_exit);
    un_pcb = queue_pop(cola_exit);
    pthread_mutex_unlock(&mutex_cola_exit);

    t_paquete* paquete = crear_paquete(ELIMINAR_PROCESO_MEMORIA);
    agregar_int_a_paquete(paquete,un_pcb ->PID);
    enviar_paquete(paquete,kernel_cliente_memoria);
    eliminar_paquete(paquete);
    borrar_registros_pcb(un_pcb);
    free(un_pcb);
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