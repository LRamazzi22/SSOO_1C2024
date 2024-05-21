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

        if(!permitir_planificacion){
            sem_wait(&detener_planificacion_to_ready);
        }
        pcb* un_pcb;
        pthread_mutex_lock(&mutex_cola_new);
        un_pcb = queue_pop(cola_new);
        pthread_mutex_unlock(&mutex_cola_new);

        un_pcb ->estado_proceso = READY;
        log_info(logger_obligatorio, "PID: %d - Estado Anterior: NEW - Estado Actual: READY", un_pcb->PID);

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
}

void planificador_exit(){
   while(true){
    sem_wait(&hay_proceso_en_exit);
    if(!permitir_planificacion){
        sem_wait(&detener_planificacion_exit);
    }
    pcb* un_pcb;
    
    pthread_mutex_lock(&mutex_cola_exit);
    un_pcb = queue_pop(cola_exit);
    pthread_mutex_unlock(&mutex_cola_exit);

    eliminar_el_proceso(un_pcb);
   }
}
