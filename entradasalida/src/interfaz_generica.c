#include <interfaz_generica.h>

void atender_peticiones_generica(){
    while(1){
        int cod_op = recibir_operacion(entradasalida_cliente_kernel);
        void * cod_op_funcion = &cod_op;
        pthread_t hilo_respuesta_generica;
        pthread_create(&hilo_respuesta_generica,NULL,(void*)ejecutar_instruccion,cod_op_funcion);
        pthread_join(hilo_respuesta_generica,NULL);
    }
}

void ejecutar_instruccion(void* cod_op){
    int * codigo = cod_op;
    t_buffer* buffer = recibir_buffer(entradasalida_cliente_kernel);
    pthread_mutex_lock(&mutex_para_interfaz);
    switch (*codigo)
    {
    case ESPERAR_GEN:
        int pid = extraer_int_buffer(buffer,logger);
        int tiempo_espera = extraer_int_buffer(buffer,logger);

        usleep(tiempo_espera*TIEMPO_UNIDAD_TRABAJO * 1000);

        t_paquete* paquete = crear_paquete(EXITO_IO);
        agregar_int_a_paquete(paquete,pid);
        enviar_paquete(paquete,entradasalida_cliente_kernel);
        eliminar_paquete(paquete);

        break;
    
    default:
        break;
    }
    pthread_mutex_unlock(&mutex_para_interfaz);

}