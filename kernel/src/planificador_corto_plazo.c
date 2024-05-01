#include <planificador_corto_plazo.h>

void iniciar_planificacion_corto_plazo(){
  pthread_t hilo_planificador_corto_plazo;
  if(strcmp(ALGORITMO_PLANIFICACION,"fifo")==0){
    pthread_create(&hilo_planificador_corto_plazo,NULL,(void*)algoritmo_fifo,NULL);
    pthread_detach(hilo_planificador_corto_plazo);
  }
  else if(strcmp(ALGORITMO_PLANIFICACION,"rr")==0){
      //pthread_create(&hilo_planificador_corto_plazo,NULL,(void*)algoritmo_round_robin,NULL);
      //pthread_detach(hilo_planificador_corto_plazo);
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

    pthread_mutex_lock(&mutex_cola_ready);
    pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);
    pthread_mutex_unlock(&mutex_cola_ready);
    
    proximo_proceso_a_ejecutar->estado_proceso = EXEC;
    proceso_en_ejecucion = proximo_proceso_a_ejecutar;

    t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    agregar_int_a_paquete(paquete_pcb_a_enviar,proximo_proceso_a_ejecutar->PID);
    serializar_registros_procesador(paquete_pcb_a_enviar, proximo_proceso_a_ejecutar->registros_cpu_en_pcb);
    
    // Enviar PCB a CPU por socket dispatch.
    enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    eliminar_paquete(paquete_pcb_a_enviar);
  
    // Esperar a que termine de ejecutar y recibir el PCB actualizado.

    atender_cpu_dispatch();
    proceso_en_ejecucion = NULL;
    
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
  int quantum = QUANTUM;
  int remaining_time = 0; // Rastrear el tiempo restante para el proceso actual

  while (queue_size(cola_ready) > 0) {
    pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);
    proximo_proceso_a_ejecutar->estado_proceso = EXEC;
    t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    // Agregar la estructura PCB al paquete
    // ...

    // Mandar PCB a la CPU a traves del  dispatch socket
    enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    eliminar_paquete(paquete_pcb_a_enviar);

    // Esperar a que el proceso termine de ejecutarse y recibir el PCB actualizado
    atender_cpu_dispatch();

    if (proceso_actual->estado_proceso == EXIT_PROCESS) {
      free(proceso_actual);
    } else {
      remaining_time = proceso_actual->tiempo_ejecucion - proceso_actual->tiempo_ejecutado;
      proceso_actual->estado_proceso = READY;
      if (remaining_time > quantum) {
        proceso_actual->tiempo_ejecutado += quantum;
      } else {
        proceso_actual->tiempo_ejecutado += remaining_time;
      }
      queue_push(cola_ready, proceso_actual);
    }


    //HILO FIFO

    //HILO LANZAR INTERRUPCION
    /*
      SLEEP(QUANTUM){
        LANZAR INTERRUPCION;
        
      } 
    */
  }
} 