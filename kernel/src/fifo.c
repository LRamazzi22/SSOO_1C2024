#include <fifo.h>

int algoritmo_fifo(){

  // Como precondicion, que la CPU esté libre. // puntero a null libre, de lo contrario
  // Proceso en running, haciendo uso de la cpu.
  while (queue_size(cola_ready) > 0) {
    // Seleccionar proceso y acutualizar estado
    pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);
    proximo_proceso_a_ejecutar->estado_proceso = EXEC;

    t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    serializar_registros_procesador(paquete_pcb_a_enviar, proximo_proceso_a_ejecutar->registros_cpu_en_pcb);
     
    // Enviar PCB a CPU por socket dispatch.
    enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    eliminar_paquete(paquete_pcb_a_enviar);

    // Esperar a que termine de ejecutar y recibir el PCB actualizado.

    atender_cpu_dispatch();
    
  }
  
  return 0;
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



/* void algoritmo_round_robin() {
  int quantum = QUANTUM;
  int remaining_time = 0; // Rastrear el tiempo restante para el proceso actual

  while (queue_size(cola_ready) > 0) {
    pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);
    proximo_proceso_a_ejecutar->estado_proceso = EXEC;
    t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);
    // Agregar la estructura PCB al paquete
    // ...

    // Mandar PCB a la CPU a través del  dispatch socket
    enviar_paquete(paquete_pcb_a_enviar, kernel_cliente_dispatch);
    free(paquete_pcb_a_enviar);

    // Esperar a que el proceso termine de ejecutarse y recibir el PCB actualizado
    atender_cpu_dispatch();

    if (proceso_actual->estado_proceso == 6) {
      free(proceso_actual);
    } else {
      remaining_time = proceso_actual->tiempo_ejecucion - proceso_actual->tiempo_ejecutado;
      if (remaining_time > quantum) {
        proceso_actual->estado_proceso = READY;
        proceso_actual->tiempo_ejecutado += quantum;
        queue_push(cola_ready, proceso_actual);
      } else {
        proceso_actual->estado_proceso = READY;
        proceso_actual->tiempo_ejecutado += remaining_time;
        queue_push(cola_ready, proceso_actual);
      }
    }
  }
} */