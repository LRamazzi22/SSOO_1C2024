#include <planificador_corto_plazo.h>

void iniciar_planificacion_corto_plazo(){
  pthread_t hilo_planificador_corto_plazo;
  if(strcmp(ALGORITMO_PLANIFICACION,"fifo")==0){
    pthread_create(&hilo_planificador_corto_plazo,NULL,(void*)algoritmo_fifo,NULL);
    pthread_detach(hilo_planificador_corto_plazo);
  }
  else if(strcmp(ALGORITMO_PLANIFICACION,"rr")==0){
      pthread_create(&hilo_planificador_corto_plazo,NULL,(void*)algoritmo_round_robin,NULL);
      pthread_detach(hilo_planificador_corto_plazo);
  }
  else if(strcmp(ALGORITMO_PLANIFICACION,"vrr")==0){
    //pthread_create(&hilo_planificador_corto_plazo,NULL,(void*)algoritmo_NULL);
    //pthread_detach(hilo_planificador_corto_plazo);
  } 
}

void algoritmo_fifo(){
  while (true) {
    // Seleccionar proceso y acutualizar estado
    sem_wait(&hay_proceso_en_ready);

    if(!permitir_planificacion){
        sem_wait(&detener_planificacion_corto_plazo);
    }

    pthread_mutex_lock(&mutex_cola_ready);
    pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);
    pthread_mutex_unlock(&mutex_cola_ready);
    
    proximo_proceso_a_ejecutar->estado_proceso = EXEC;
    log_info(logger_obligatorio, "PID: %d - Estado Anterior: READY - Estado Actual: EXECUTE", proximo_proceso_a_ejecutar->PID);
    pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
    proceso_en_ejecucion = proximo_proceso_a_ejecutar;
    pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

    t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    agregar_int_a_paquete(paquete_pcb_a_enviar,proximo_proceso_a_ejecutar->PID);
    serializar_registros_procesador(paquete_pcb_a_enviar, proximo_proceso_a_ejecutar->registros_cpu_en_pcb);
    
    // Enviar PCB a CPU por socket dispatch.
    enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    eliminar_paquete(paquete_pcb_a_enviar);
  
    // Esperar a que termine de ejecutar y recibir el PCB actualizado.

    atender_cpu_dispatch();
    pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
    proceso_en_ejecucion = NULL;
    pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);
    
  }
  
}

//Tampoco hacia falta despues vemos si lo sacamos

void serializar_registros_procesador (t_paquete* paquete, t_registros_cpu* proceso) {
    agregar_a_paquete(paquete,proceso->PC,sizeof(uint32_t));
    agregar_a_paquete(paquete,proceso->AX,sizeof(uint8_t));
    agregar_a_paquete(paquete,proceso->BX,sizeof(uint8_t));
    agregar_a_paquete(paquete,proceso->CX,sizeof(uint8_t));
    agregar_a_paquete(paquete,proceso->DX,sizeof(uint8_t));
    agregar_a_paquete(paquete,proceso->EAX,sizeof(uint32_t));
    agregar_a_paquete(paquete,proceso->EBX,sizeof(uint32_t)); 
    agregar_a_paquete(paquete,proceso->ECX,sizeof(uint32_t));
    agregar_a_paquete(paquete,proceso->EDX,sizeof(uint32_t));
    agregar_a_paquete(paquete,proceso->SI,sizeof(uint32_t)); 
    agregar_a_paquete(paquete,proceso->DI,sizeof(uint32_t));
}



 void algoritmo_round_robin() {
   while (true) {
    // Seleccionar proceso y acutualizar estado
    sem_wait(&hay_proceso_en_ready);

    pthread_mutex_lock(&mutex_cola_ready);
    pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);
    pthread_mutex_unlock(&mutex_cola_ready);
    
    proximo_proceso_a_ejecutar->estado_proceso = EXEC;
    log_info(logger_obligatorio, "PID: %d - Estado Anterior: READY - Estado Actual: EXECUTE", proximo_proceso_a_ejecutar->PID);

    pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
    proceso_en_ejecucion = proximo_proceso_a_ejecutar;
    pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);

    t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    agregar_int_a_paquete(paquete_pcb_a_enviar,proximo_proceso_a_ejecutar->PID);
    serializar_registros_procesador(paquete_pcb_a_enviar, proximo_proceso_a_ejecutar->registros_cpu_en_pcb);
    
    // Enviar PCB a CPU por socket dispatch.
    enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    eliminar_paquete(paquete_pcb_a_enviar);
    pthread_create(&(proximo_proceso_a_ejecutar ->hilo_quantum),NULL,(void*)esperar_quantum,(void*)proximo_proceso_a_ejecutar);
    pthread_detach(proximo_proceso_a_ejecutar ->hilo_quantum);
    proceso_en_ejecucion -> tiempo_en_ejecucion = temporal_create();

  
    // Esperar a que termine de ejecutar y recibir el PCB actualizado.

    atender_cpu_dispatch();
    pthread_mutex_lock(&mutex_para_proceso_en_ejecucion);
    proceso_en_ejecucion = NULL;
    pthread_mutex_unlock(&mutex_para_proceso_en_ejecucion);
    
  }
}

void esperar_quantum(void* pcb_referencia){
  pcb* un_pcb = pcb_referencia;
  usleep(un_pcb ->quantum_restante * 1000);
  if(un_pcb ->PID == proceso_en_ejecucion ->PID){
    t_paquete* paquete = crear_paquete(INTERRUPCION);
    agregar_int_a_paquete(paquete, un_pcb ->PID);
    enviar_paquete(paquete,kernel_cliente_interrupt);
    eliminar_paquete(paquete);
  }

}