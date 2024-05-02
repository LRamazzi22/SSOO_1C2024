#include <interfaz_generica.h>

void atender_peticiones_generica(){
    while(1){
        int cod_op = recibir_operacion(entradasalida_cliente_kernel);
        switch(cod_op){
            case ESPERAR_GEN:
                t_buffer* buffer = recibir_buffer(entradasalida_cliente_kernel);
                pthread_t hilo_respuesta_generica;
                pthread_create(&hilo_respuesta_generica,NULL,(void*)ejecutar_instruccion,(void*)buffer);
                pthread_detach(hilo_respuesta_generica);
                break;

            default:
                break;
        }
    }
}

void ejecutar_instruccion(void* buffer_void){
    t_buffer* buffer = buffer_void;
    int pid = extraer_int_buffer(buffer,logger);
    int tiempo_espera = extraer_int_buffer(buffer,logger);

    usleep(tiempo_espera*TIEMPO_UNIDAD_TRABAJO * 1000);

    t_paquete* paquete = crear_paquete(EXITO_IO);
    agregar_int_a_paquete(paquete,pid);
    enviar_paquete(paquete,entradasalida_cliente_kernel);
    eliminar_paquete(paquete);

}

