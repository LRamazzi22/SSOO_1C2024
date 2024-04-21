#include <main_memoria.h>

int main(int argc, char* argv[]) {

    inicializar_memoria();

    if(TAM_MEMORIA % TAM_PAGINA != 0){
		log_error(logger, "Tamaño de memoria no es multiplo de tamaño de memoria");
		exit(EXIT_FAILURE);
	}
	void* reserva_memoria = malloc(TAM_MEMORIA);
	cantidad_instrucciones = 0;
	

	/* Descomentar el bloque si tiene un archivo .txt en el path adecuado.
	char* nombre = "Pseudo.txt";
	int p= leer_archivo(nombre);
	printf("%s",lista_de_instrucciones[0]);
	printf("%s",lista_de_instrucciones[1]);
	*/
	
	

	//Se inicia la memoria como servidor
    memoria_server = iniciar_servidor(PUERTO_ESCUCHA, logger);
	
	//Esperar la conexion del cpu
	cpu_cliente = esperar_cliente(memoria_server, logger, "Cpu conectada");
	

	//Esperar la conexion del kernel
	kernel_cliente = esperar_cliente(memoria_server,logger, "Kernel Conectado");

	//Esperar la conexion de la Entrada/Salida
	entradasalida_cliente = esperar_cliente(memoria_server, logger, "Entrada/Salida Conectado");
	
	//Atender mensajes del cpu
	pthread_t hilo_cpu;
	pthread_create(&hilo_cpu,NULL,(void*)atender_cpu_memoria, NULL);
	pthread_detach(hilo_cpu);

	//Atender mensajes del kernel
	pthread_t hilo_kernel;
	pthread_create(&hilo_kernel,NULL,(void*)atender_kernel_memoria, NULL);
	pthread_detach(hilo_kernel);

	//Atender mensajes de Entrada/Salida
	pthread_t hilo_entradasalida;
	pthread_create(&hilo_entradasalida,NULL,(void*)atender_entradasalida_memoria, NULL);
	pthread_join(hilo_entradasalida, NULL);

	liberar_conexion(cpu_cliente);
	liberar_conexion(kernel_cliente);
	liberar_conexion(entradasalida_cliente);
	liberar_conexion(memoria_server);
	
	terminar_programa(logger, config);
	free(reserva_memoria);
	return EXIT_SUCCESS;

	
}
