#include <eliminar_proceso.h>

void eliminar_el_proceso(pcb* pcb_a_finalizar){
    pcb_a_finalizar->estado_proceso = EXIT_PROCESS;

	pthread_mutex_lock(&mutex_cola_exit);
	queue_push(cola_exit,pcb_a_finalizar);
	pthread_mutex_unlock(&mutex_cola_exit);
	sem_post(&hay_proceso_en_exit);

}