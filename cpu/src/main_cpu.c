#include <main_cpu.h>

int main(int argc, char* argv[]) {

    inicializar_cpu();

    //Iniciar CPU como Server
    cpu_server_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, logger);
    cpu_server_interrupt = iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT, logger);

    //Conectarse a la memoria
    cpu_cliente_memoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    

    //Esperar que se conecte el kernel
    kernel_cliente_dispatch = esperar_cliente(cpu_server_dispatch, logger, "Kernel dispatch conectado");
    kernel_cliente_interrupt = esperar_cliente(cpu_server_interrupt, logger, "Kernel interrupt conectado");


    //Atender mensajes del Kernel Dispatch
    pthread_t hilo_kernel_dispatch;
	pthread_create(&hilo_kernel_dispatch,NULL,(void*)atender_kernel_dispatch, NULL);
	pthread_detach(hilo_kernel_dispatch);

    //Atender mensajes del Kernel Interrupt
    pthread_t hilo_kernel_interrupt;
	pthread_create(&hilo_kernel_interrupt,NULL,(void*)atender_kernel_interrupt, NULL);
	pthread_detach(hilo_kernel_interrupt);

    //Atender mensajes de la Memoria Server
	pthread_t hilo_memoria_server;
	pthread_create(&hilo_memoria_server,NULL,(void*)atender_memoria_cpu, NULL);
	pthread_detach(hilo_memoria_server);

    enviar_handshake("CPU", cpu_cliente_memoria);

    printf("\nUna vez conectados todos los modulos en el orden establecido en el README, presione ENTER para iniciar la prueba\n\n");
    char hola = getchar();

    prueba_de_protocolo();


    terminar_programa(logger, config);
    return 0;
}

void prueba_de_protocolo(){
    t_paquete* paquete = crear_paquete(PROTOCOLO);
    int num = 23;
    agregar_a_paquete(paquete, &num, sizeof(int));
    char * mensaje = "Hola como estas, soy el cpu.\n\n";
    agregar_a_paquete(paquete, mensaje, strlen(mensaje)+1);
    enviar_paquete(paquete, cpu_cliente_memoria);
    eliminar_paquete(paquete);
}


