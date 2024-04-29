#include <main_entradasalida.h>

int main(int argc, char* argv[]) {
     
    char* nombre_interfaz = malloc(50);
    char* path_config = malloc(256);
    printf("Ingrese el nombre de la interfaz\n");
    fgets(nombre_interfaz,50,stdin);
    printf("Ingrese el path del config con el que desea inicializar la interfaz\n");
    fgets(path_config,256,stdin);
    inicializar_entradasalida(path_config);
    free(path_config);

    //Se conecta al Kernel
    entradasalida_cliente_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    t_paquete* paquete = crear_paquete(PRIMERA_CONEXION_IO);
    agregar_string_a_paquete(paquete,nombre_interfaz);
    agregar_string_a_paquete(paquete,TIPO_INTERFAZ);
    enviar_paquete(paquete,entradasalida_cliente_kernel);
    eliminar_paquete(paquete);
    
    if(strcmp(TIPO_INTERFAZ,"Generica")!=0){
        //Se conecta al Memoria 
        entradasalida_cliente_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
        enviar_handshake("Entrada/Salida", entradasalida_cliente_memoria);
    }
    
    enviar_handshake("Entrada/Salida", entradasalida_cliente_kernel);

    //Atender mensajes del Kernel
    pthread_t hilo_kernel_entradasalida;
	pthread_create(&hilo_kernel_entradasalida,NULL,(void*)atender_kernel_entradasalida, NULL);
	pthread_join(hilo_kernel_entradasalida,NULL);
    

    //Atender mensajes de la Memoria
	//pthread_t hilo_memoria_entradasalida;
	//pthread_create(&hilo_memoria_entradasalida,NULL,(void*)atender_memoria_entradasalida, NULL);
	//pthread_join(hilo_memoria_entradasalida, NULL);

    liberar_conexion(entradasalida_cliente_kernel);
    //liberar_conexion(entradasalida_cliente_memoria);
    
    terminar_programa(logger, config);


    return 0;
    
}