#include <fifo.h>

int algoritmo_fifo(){

      //Como precondicion, que la CPU esté libre. (TODO)

    while (queue_size(cola_ready) > 0) {
     
     pcb* proximo_proceso_a_ejecutar = queue_pop(cola_ready);

     proximo_proceso_a_ejecutar->estado_proceso = EXEC;

     t_paquete* paquete_pcb_a_enviar = crear_paquete(INICIAR_EXEC);

     // Se debe agregar al paquete la estructura PCB... Existen atributos dinámicos... (Agregar tamaño de c/u?).
     
     // Enviar PCB a CPU por socket dispatch.
     enviar_paquete(paquete_pcb_a_enviar,kernel_cliente_dispatch);

     free(paquete_pcb_a_enviar);

     // Esperar a que termine de ejecutar y recibir el PCB actualizado.

    atender_cpu_dispatch();
    
    }

    return 0;


}