#include <interfaz_dialfs.h>



void atender_peticiones_dialfs(){
    while(1){
        int cod_op = recibir_operacion(entradasalida_cliente_kernel);
        t_buffer* buffer;
        switch(cod_op){
            case FS_CREATE_CODE:
                buffer = recibir_buffer(entradasalida_cliente_kernel);
                int pid1 = extraer_int_buffer(buffer, logger);
                char* nombre_Archivo1 = extraer_string_buffer(buffer, logger);

                int bloque_elegido = buscar_bloque_libre();

                if(bloque_elegido == -1){
                    t_paquete* paquete = crear_paquete(FALLO_IO);
                    agregar_int_a_paquete(paquete,pid1);
                    enviar_paquete(paquete,entradasalida_cliente_kernel);
                    eliminar_paquete(paquete);
                }
                else{
                    char* path_archivo = strdup(PATH_BASE_DIALFS);
                    string_append(&path_archivo, nombre_Archivo1);

                    FILE* metadatos = fopen(path_archivo, "r");

                    if(metadatos == NULL){
                        metadatos = fopen(path_archivo, "w");
                        fclose(metadatos);

                        t_config* meta_config = config_create(path_archivo);

                        config_set_value(meta_config, "BLOQUE_INICIAL", string_itoa(bloque_elegido));
                        config_set_value(meta_config, "TAMANIO_ARCHIVO", string_itoa(BLOCK_SIZE));

                        config_save(meta_config);

                        config_destroy(meta_config);

                        bitarray_set_bit(bitmap_bloques,bloque_elegido);
                    }
                    else{
                        log_info(logger, "Se intenta crear un archivo que ya existe, no se hara nada");
                    }

                    free(path_archivo);

                    t_paquete* paquete = crear_paquete(EXITO_IO);
                    agregar_int_a_paquete(paquete,pid1);
                    enviar_paquete(paquete,entradasalida_cliente_kernel);
                    eliminar_paquete(paquete);
                }

                free(nombre_Archivo1);
                               
                break;           
            case FS_DELETE_CODE:
                buffer = recibir_buffer(entradasalida_cliente_kernel);
                int pid2 = extraer_int_buffer(buffer, logger);
                char* nombre_Archivo2 = extraer_string_buffer(buffer, logger);

                char* path_archivo2 = strdup(PATH_BASE_DIALFS);
                string_append(&path_archivo2, nombre_Archivo2);

                t_config* meta_config_delete = config_create(path_archivo2);

                if(meta_config_delete != NULL){
                    int primera_bloque = config_get_int_value(meta_config_delete, "BLOQUE_INICIAL");
                    int tamanio_total = config_get_int_value(meta_config_delete, "TAMANIO_ARCHIVO");

                    float tamanio_total_float = tamanio_total;

                    int cant_bloques = ceil(tamanio_total_float / BLOCK_SIZE);

                    for(int i = 0; i < cant_bloques; i++){
                        bitarray_clean_bit(bitmap_bloques,primera_bloque);
                        primera_bloque++;
                    }

                    config_destroy(meta_config_delete);

                    remove(path_archivo2);


                }
                else{
                    log_info(logger, "Se intenta borrar un archivo que no existe, no se hara nada");
                }

                t_paquete* paquete = crear_paquete(EXITO_IO);
                agregar_int_a_paquete(paquete,pid2);
                enviar_paquete(paquete,entradasalida_cliente_kernel);
                eliminar_paquete(paquete);

                free(nombre_Archivo2);
                free(path_archivo2);
                
                break;
            case FS_TRUNCATE_CODE:
                buffer = recibir_buffer(entradasalida_cliente_kernel);
                int pid3 = extraer_int_buffer(buffer, logger);
                char* nombre_Archivo3 = extraer_string_buffer(buffer, logger);
                int tamanio_a_truncar = extraer_int_buffer(buffer, logger);
            
                break;
            case FS_READ_CODE:
                buffer = recibir_buffer(entradasalida_cliente_kernel);
                int pid4 = extraer_int_buffer(buffer,logger);
                char* nombre_Archivo4 = extraer_string_buffer(buffer, logger);
                int puntero_Arch4 = extraer_int_buffer(buffer, logger);
                int tam_tota4l = extraer_int_buffer(buffer,logger);
                int cant_dir_fisicas4 = extraer_int_buffer(buffer, logger);


                void* leido_de_fs; //ACA HAY QUE HACER LA FUNCION QUE LEE EL CONTENIDO DEL FS

                int desplazamiento4 = 0;

                for(int i = 0; i< cant_dir_fisicas4; i++){
                    int dir_fisca = extraer_int_buffer(buffer, logger);
                    int tam = extraer_int_buffer(buffer, logger);

                    void* copiar_a_mem = malloc(tam);

                    memcpy(copiar_a_mem,leido_de_fs + desplazamiento4, tam);

                    desplazamiento4 = desplazamiento4 + tam;

                    t_paquete* paquete1 = crear_paquete(ESCRITURA_CODE);

                    agregar_int_a_paquete(paquete1, pid4);
                    agregar_int_a_paquete(paquete1, dir_fisca);
                    agregar_int_a_paquete(paquete1,tam);

                    agregar_a_paquete(paquete1, copiar_a_mem, tam);
                    enviar_paquete(paquete1, entradasalida_cliente_memoria);
                    eliminar_paquete(paquete1);
                    free(copiar_a_mem);
                    
                }

                free(leido_de_fs);


                break;
                
            case FS_WRITE_CODE:
                buffer = recibir_buffer(entradasalida_cliente_kernel);
                int pid5 = extraer_int_buffer(buffer,logger);
                char* nombre_Archivo5 = extraer_string_buffer(buffer, logger);
                int puntero_Arch5 = extraer_int_buffer(buffer, logger);
                int tam_total5 = extraer_int_buffer(buffer,logger);
                int cant_dir_fisicas5 = extraer_int_buffer(buffer, logger);
                
                void* contenido = malloc(tam_total5 + 1);

                int desplazamiento5 = 0;

                for(int i = 0; i< cant_dir_fisicas5; i++){
                    int dir_fisca = extraer_int_buffer(buffer, logger);
                    int tam = extraer_int_buffer(buffer, logger);

                    t_paquete* paquete1 = crear_paquete(LECTURA_CODE);

                    agregar_int_a_paquete(paquete1, pid5);
                    agregar_int_a_paquete(paquete1, dir_fisca);
                    agregar_int_a_paquete(paquete1,tam);

                    enviar_paquete(paquete1, entradasalida_cliente_memoria);
                    eliminar_paquete(paquete1);
                    
                    void* leido = recibir_lectura();

                    memcpy(contenido + desplazamiento5, leido, tam);

                    desplazamiento5 = desplazamiento5 + tam;

                    free(leido);
                }

                break;
            default:
                break;
        }
    }
}


void levantar_archivos(){
    int tamanio_archivo = BLOCK_SIZE * BLOCK_COUNT;
    char* nombre_archivo_bloques = strdup(PATH_BASE_DIALFS);
    string_append(&nombre_archivo_bloques, "bloques.dat");

    Archivo_bloques = fopen(nombre_archivo_bloques, "r+b");

    if(Archivo_bloques == NULL){
        Archivo_bloques = fopen(nombre_archivo_bloques, "w+b");

        truncate(nombre_archivo_bloques,tamanio_archivo);
    }
    else{
        fseek(Archivo_bloques, 0, SEEK_END);
        int tam = ftell(Archivo_bloques);
        fseek(Archivo_bloques, 0, SEEK_SET);
        
        if(tam != tamanio_archivo){
            truncate(nombre_archivo_bloques, tamanio_archivo);
        }
    }

    int fd_bloques = fileno(Archivo_bloques);

    archivo_bloques_en_mem = mmap(NULL,tamanio_archivo, PROT_READ | PROT_WRITE, MAP_SHARED, fd_bloques, 0);

    fclose(Archivo_bloques);

    free(nombre_archivo_bloques);

    char* nombre_archivo_bitmap = strdup(PATH_BASE_DIALFS);
    string_append(&nombre_archivo_bitmap, "bitmap.dat");

    Archivo_bitmap = fopen(nombre_archivo_bitmap, "r+b");

    float cant_block_float = BLOCK_COUNT;

    int tamanio_bitmap = ceil(cant_block_float/8);

    bool creacion = false;

    if(Archivo_bitmap == NULL){
        Archivo_bitmap = fopen(nombre_archivo_bitmap, "w+b");

        truncate(nombre_archivo_bitmap,tamanio_bitmap);

        creacion = true;
    }

    int fd_bitmap = fileno(Archivo_bitmap);

    puntero_a_bits_de_bloques = mmap(NULL, tamanio_bitmap, PROT_READ | PROT_WRITE, MAP_SHARED, fd_bitmap, 0);

     fclose(Archivo_bloques);

    bitmap_bloques = bitarray_create_with_mode(puntero_a_bits_de_bloques, tamanio_bitmap, LSB_FIRST);

    if(creacion){
        for(int i = 0; i < BLOCK_COUNT; i++){
            bitarray_clean_bit(bitmap_bloques,i);
        }
    }

    free(nombre_archivo_bitmap);
    

}

int buscar_bloque_libre(){
    int i;

    for(i = 0; i < BLOCK_COUNT; i++){
        if(!bitarray_test_bit(bitmap_bloques,i)){
            return i;
        }
    }

    return -1;
}