#include <atender_nuevas_interfaces.h>



void atender_nueva_interfaz(void* cliente_entradasalida){
    int* cliente = cliente_entradasalida;
    int cod_op = recibir_operacion(*cliente);
    if(cod_op == PRIMERA_CONEXION_IO){
        t_buffer* buffer = recibir_buffer(*cliente);
        char* nombre_interfaz = extraer_string_buffer(buffer,logger);
        char* tipo_interfaz = extraer_string_buffer(buffer,logger);
        nodo_de_diccionario_interfaz* nodo = malloc(sizeof(nodo_de_diccionario_interfaz));
        nodo ->tipo_de_interfaz = tipo_interfaz;
        nodo ->cliente = *cliente;

        dictionary_put(diccionario_entrada_salida,nombre_interfaz,nodo);
        nombre_y_cliente* nombre_cliente= malloc(sizeof(nombre_y_cliente));
        nombre_cliente ->cliente = *cliente;
        nombre_cliente ->nombre = nombre_interfaz;
        pthread_t hilo_entradasalida_desconexion;
        pthread_create(&hilo_entradasalida_desconexion,NULL, (void*)atender_cierre_de_interfaz,(void*)nombre_cliente);
        pthread_join(hilo_entradasalida_desconexion,NULL);

    }
    else{
        log_error(logger,"Se envio el mensaje equivocado");
        liberar_conexion(*cliente);
    }
    free(cliente);
}

void atender_cierre_de_interfaz(void* nombre_interfaz_y_cliente){
    nombre_y_cliente* nombre_cliente = nombre_interfaz_y_cliente;
    bool liberar_while = false;
    while(!liberar_while){

        int cod_op = recibir_operacion(nombre_cliente ->cliente);
        switch (cod_op)
        {
        case PRIMERA_CONEXION_IO:
            break;
        case -1:
            nodo_de_diccionario_interfaz* nodo = dictionary_remove(diccionario_entrada_salida,nombre_cliente ->nombre);
            free(nodo ->tipo_de_interfaz);
            liberar_conexion(nodo ->cliente);
            free(nodo);
            liberar_while = true;
            break;
    
        default:
            break;
        }
    }

}

void atender_las_nuevas_interfaces(){
    int* cliente;
    while(1){
        cliente = malloc(sizeof(int));
        *cliente = esperar_cliente(kernel_server, logger, "Entrada Salida Conectado");
        pthread_t hilo_atender_entradasalida;
        pthread_create(&hilo_atender_entradasalida,NULL, (void*)atender_nueva_interfaz,(void*)cliente);
        pthread_detach(hilo_atender_entradasalida);
    }
    
}