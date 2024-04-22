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
    /*
	pthread_t hilo_memoria_server;
	pthread_create(&hilo_memoria_server,NULL,(void*)atender_memoria_cpu, NULL);
	pthread_detach(hilo_memoria_server);
    */

    enviar_handshake("CPU", cpu_cliente_memoria);

    printf("\nUna vez conectados todos los modulos en el orden establecido en el Instruciones.MD, presione ENTER para iniciar la prueba\n\n");
    char hola = getchar();

    prueba_de_protocolo();

    //solicitar_instruccion(0);

    liberar_conexion(cpu_cliente_memoria);
    liberar_conexion(cpu_server_dispatch);
    liberar_conexion(cpu_server_interrupt);

    terminar_programa(logger, config);
    return 0;
}

void prueba_de_protocolo(){
    t_paquete* paquete = crear_paquete(PROTOCOLO);
    int num = 23;
    agregar_int_a_paquete(paquete,num);
    char * mensaje = "Hola como estas, soy el cpu.\n\n";
    agregar_string_a_paquete(paquete, mensaje);
    enviar_paquete(paquete, cpu_cliente_memoria);
    eliminar_paquete(paquete);
}

void solicitar_instruccion(int programCounter){
    t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION);
    agregar_int_a_paquete(paquete, programCounter);
    enviar_paquete(paquete, cpu_cliente_memoria);
    eliminar_paquete(paquete);
    atender_memoria_cpu_sin_while();
    //printf("%s",instruccion_a_decodificar);
}


t_registros_cpu* iniciar_registros_cpu(){
    t_registros_cpu* registro = malloc(sizeof(t_registros_cpu));

    registro->pc = malloc(sizeof(4));
    registro->ax = malloc(sizeof(1));
    registro->bx = malloc(sizeof(1));
    registro->cx = malloc(sizeof(1));
    registro->dx = malloc(sizeof(1));
    registro->eax = malloc(sizeof(4));
    registro->ebx = malloc(sizeof(4));
    registro->ecx = malloc(sizeof(4));
    registro->edx = malloc(sizeof(4));
    registro->si = malloc(sizeof(4));
    registro->di = malloc(sizeof(4));

    registro->pc = 0;
    registro->ax = NULL;
    registro->bx = NULL;
    registro->cx = NULL;
    registro->dx = NULL;
    registro->eax = NULL;
    registro->ebx = NULL;
    registro->ecx = NULL;
    registro->edx = NULL;
    registro->si = NULL;
    registro->di = NULL;

    return registro;

}

//Hay que pulir mucho pseucodigo jej
void fetch(/*Aca podria haber una estructura pcb o del contexto de ejecucion*/){
    //instruccion_a_ejecutar = lista_instrucciones[pc] -> esto esta hecho con la lista de instrucciones que tenemos podriamos tener lista encadenada de nodos
    //pc++;
    //decode(instruccion);
}

void decode(){
    //TODO
    //aca iria un switch con casos que pide la consigna como instrucciones 

}

void execute(){
    //TODO
}