#include <main_entradasalida.h>

int main(int argc, char* argv[]) {
     
    inicializar_entradasalida();

    //Se conecta al Kernel
    entradasalida_cliente_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    
    //Se conecta al Memoria 
    entradasalida_cliente_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    //Atender mensajes del Kernel
    pthread_t hilo_kernel_entradasalida;
	pthread_create(&hilo_kernel_entradasalida,NULL,(void*)atender_kernel_entradasalida, NULL);
	pthread_detach(hilo_kernel_entradasalida);

    enviar_handshake("Entrada/Salida", entradasalida_cliente_kernel);
    enviar_handshake("Entrada/Salida", entradasalida_cliente_memoria);

    //Atender mensajes de la Memoria
	pthread_t hilo_memoria_entradasalida;
	pthread_create(&hilo_memoria_entradasalida,NULL,(void*)atender_memoria_entradasalida, NULL);
	pthread_join(hilo_memoria_entradasalida, NULL);

    liberar_conexion(entradasalida_cliente_kernel);
    liberar_conexion(entradasalida_cliente_memoria);
    
    terminar_programa(logger, config);


    return 0;
    
}