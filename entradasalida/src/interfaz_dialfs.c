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
                               
                break;           
            case FS_DELETE_CODE:
                buffer = recibir_buffer(entradasalida_cliente_kernel);
                int pid2 = extraer_int_buffer(buffer, logger);
                char* nombre_Archivo2 = extraer_string_buffer(buffer, logger);
                
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
    string_append(&nombre_archivo_bloques, "/bloques.dat");

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
    string_append(&nombre_archivo_bitmap, "/bitmap.dat");

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
