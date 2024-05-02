#include <main_kernel.h>

int main(int argc, char* argv[]) {

    inicializar_kernel();
    diccionario_entrada_salida = dictionary_create();
    
    //Se inicia al kernel como servidor
    kernel_server = iniciar_servidor(PUERTO_ESCUCHA, logger);

    //Se conecta Kernel a memoria
    kernel_cliente_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    //Se conecta el kernel con el CPU dispatch
    kernel_cliente_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);

    //Se conecta el kernel con el CPU interrupt
    kernel_cliente_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);

    //Esperar conexion de Entrada Salida
    //entradasalida_cliente = esperar_cliente(kernel_server, logger, "Entrada Salida Conectado");
    
    
    pthread_t hilo_entradasalida_kernel;
	pthread_create(&hilo_entradasalida_kernel,NULL,(void*)atender_las_nuevas_interfaces, NULL);
	pthread_detach(hilo_entradasalida_kernel);

    enviar_handshake("Kernel",kernel_cliente_memoria);
    enviar_handshake("Kernel Interrupt", kernel_cliente_interrupt);

    iniciar_planificacion_largo_plazo();
    iniciar_planificacion_corto_plazo();
    
    //Atender mensajes del Entrada Salida
    
    


    
    //Atender mensajes de Memoria
    //pthread_t hilo_memoria_kernel;
	//pthread_create(&hilo_memoria_kernel,NULL,(void*)atender_memoria, NULL);
	//pthread_detach(hilo_memoria_kernel);

    //Atender mensajes de CPU Dispatch
    //pthread_t hilo_dispatch_kernel;
	//pthread_create(&hilo_dispatch_kernel,NULL,(void*)atender_cpu_dispatch, NULL);
	//pthread_detach(hilo_dispatch_kernel);

    sleep(1);

    consola_kernel();
    

    liberar_conexion(kernel_cliente_dispatch);
    liberar_conexion(kernel_cliente_interrupt);
    //liberar_conexion(entradasalida_cliente);
    liberar_conexion(kernel_server);
    terminar_programa(logger, config);
    return 0;
}
