#include <ciclo_cpu.h>
#include <atender_mensajes.h>

void ciclo(){
        
    atender_kernel_dispatch_sin_while();
    bool correr_ciclo = true;
    int resultado_ejecucion;
    while(correr_ciclo){
        solicitar_instruccion(*los_registros_de_la_cpu -> PC);
        int cod_instruccion = decodificar_instruccion();
        resultado_ejecucion = ejecutar_instruccion(cod_instruccion);
        uint32_t programCounter = *los_registros_de_la_cpu -> PC;
        programCounter++;
        *los_registros_de_la_cpu -> PC = programCounter;
        if(resultado_ejecucion != SEGUIR_EJECUTANDO){
            correr_ciclo = false;

            pthread_mutex_lock(&mutex_para_interrupcion);
            interrupcion_recibida = NO_INTERRUPCION;
            pthread_mutex_unlock(&mutex_para_interrupcion);
        }
        else if(interrupcion_recibida != NO_INTERRUPCION && pid_en_ejecucion == pid_de_interrupcion){
            correr_ciclo = false;
        }
        else{
            string_array_destroy(instruccion_separada);
        }
    }
    if(resultado_ejecucion != SEGUIR_EJECUTANDO){
        switch (resultado_ejecucion){
            case FINALIZAR:
                t_paquete* paquete = crear_paquete(FINALIZAR_EXEC);
                cargar_registros_a_paquete(paquete);
                enviar_paquete(paquete,kernel_cliente_dispatch);
                eliminar_paquete(paquete);

                break;
            case SLEEP_GEN:
                t_paquete* paquete2 = crear_paquete(ESPERAR_GEN);
                cargar_registros_a_paquete(paquete2);
                string_append(&instruccion_separada[1],"\n");
                agregar_string_a_paquete(paquete2,instruccion_separada[1]);
                int tiempo_espera = atoi(instruccion_separada[2]);
                agregar_int_a_paquete(paquete2,tiempo_espera);
                enviar_paquete(paquete2,kernel_cliente_dispatch);
                eliminar_paquete(paquete2);

                break;
            case WAIT_RECURSO:
                t_paquete* paquete3 = crear_paquete(WAIT_CODE);
                cargar_registros_a_paquete(paquete3);
                agregar_string_a_paquete(paquete3,instruccion_separada[1]);
                enviar_paquete(paquete3,kernel_cliente_dispatch);
                eliminar_paquete(paquete3);
                break;
            case SIGNAL_RECURSO:
                t_paquete* paquete4 = crear_paquete(SIGNAL_CODE);
                cargar_registros_a_paquete(paquete4);
                agregar_string_a_paquete(paquete4,instruccion_separada[1]);
                enviar_paquete(paquete4,kernel_cliente_dispatch);
                eliminar_paquete(paquete4);
                break;
            case SIN_MEMORIA:
                t_paquete* paquete5 = crear_paquete(OUT_OF_MEM_CODE);
                cargar_registros_a_paquete(paquete5);
                enviar_paquete(paquete5,kernel_cliente_dispatch);
                eliminar_paquete(paquete5);
                break;
            default:
                break;
        }
    }
    else{
        t_paquete* paquete = crear_paquete(INTERRUPCION);
        cargar_registros_a_paquete(paquete);
        enviar_paquete(paquete,kernel_cliente_dispatch);
        eliminar_paquete(paquete);
            
        pthread_mutex_lock(&mutex_para_interrupcion);
        interrupcion_recibida = NO_INTERRUPCION;
        pthread_mutex_unlock(&mutex_para_interrupcion);
        
        
    }
    
    string_array_destroy(instruccion_separada);
}

void solicitar_instruccion(int programCounter){
    t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION);
    agregar_int_a_paquete(paquete,pid_en_ejecucion);
    agregar_int_a_paquete(paquete, programCounter);
    enviar_paquete(paquete, cpu_cliente_memoria);
    log_info(logger_obligatorio, "PID: %d - FETCH Program Counter: %d", pid_en_ejecucion, programCounter);
    eliminar_paquete(paquete);
    atender_memoria_cpu_sin_while();
    printf("%s",instruccion_a_decodificar);
}

int decodificar_instruccion(){
    // instruccion_a_decodificar
    instruccion_separada = string_split(instruccion_a_decodificar, " ");

    if (!strcmp(instruccion_separada[0],"SET")) {
        return SET;
        
    } else if (!strcmp(instruccion_separada[0],"MOV_IN")) {
        return MOV_IN;
        
    } else if (!strcmp(instruccion_separada[0], "MOV_OUT")) {
        return MOV_OUT;
        
    } else if (!strcmp(instruccion_separada[0], "SUM")) {
        return SUM;

    } else if (!strcmp(instruccion_separada[0], "SUB")) {
        return SUB;

    } else if (!strcmp(instruccion_separada[0], "JNZ")) {
        return JNZ;

    } else if (!strcmp(instruccion_separada[0], "RESIZE")) {
        return RESIZE;
        
    } else if (!strcmp(instruccion_separada[0], "COPY_STRING")) {
        return COPY_STRING;
        
    } else if (!strcmp(instruccion_separada[0], "WAIT")) {
        return WAIT;
        
    } else if (!strcmp(instruccion_separada[0],"SIGNAL")) {
        return SIGNAL;
        
    } else if (!strcmp(instruccion_separada[0],"IO_GEN_SLEEP")) {
        return IO_GEN_SLEEP;
           
    } else if (!strcmp(instruccion_separada[0],"IO_STDIN_READ")) {
        return IO_STDIN_READ;
        
    } else if (!strcmp(instruccion_separada[0],"IO_STDOUT_WRITE")) {
        return IO_STDOUT_WRITE;
        
    } else if (!strcmp(instruccion_separada[0],"IO_FS_CREATE")) {
        return IO_FS_CREATE;
        
    } else if (!strcmp(instruccion_separada[0],"IO_FS_DELETE")) {
        return IO_FS_DELETE;
        
    } else if (!strcmp(instruccion_separada[0],"IO_FS_TRUNCATE")) {
        return IO_FS_TRUNCATE;
        
    } else if (!strcmp(instruccion_separada[0],"IO_FS_WRITE")) {
        return IO_FS_WRITE;
        
    } else if (!strcmp(instruccion_separada[0],"IO_FS_READ")) {
        return IO_FS_READ;
        
    } else if (!strcmp(instruccion_separada[0], "EXIT")) {
        return EXIT;
        
    } else {
        log_error(logger, "Decode: Comando no reconocido");
        return -1;
    }
}

void set(char* nombre_registro, int num){
    int tamano = 0;
    void* registro = apuntar_a_registro(nombre_registro, &tamano);
    if (registro == NULL) {
        printf("Es NULL\n");
    }
    if (tamano == 8) {
        int8_t* registro2 = registro;
        *registro2 = num;
    } else if (tamano == 32) {
        int32_t* registro2 = registro;
        *registro2 = num;
    }
}

void sum(char* nombre_registro_destino, char* nombre_registro_origen){
    int tamano_origen = 0;
    void* registro_origen = apuntar_a_registro(nombre_registro_origen, &tamano_origen);
    int contenido_origen = 0;

    if (tamano_origen == 8) {
        int8_t* registro_origen2 = registro_origen;
        contenido_origen = *registro_origen2;
        
    } else if (tamano_origen == 32) {
        int32_t* registro_origen2 = registro_origen;
        contenido_origen = *registro_origen2;
    }

    int tamano_destino = 0;
    void* registro_destino = apuntar_a_registro(nombre_registro_destino, &tamano_destino);

    if (tamano_destino == 8) {
        int8_t* registro_destino2 = registro_destino;
        *registro_destino2 += contenido_origen;
        printf("%d\n", *registro_destino2);
    } else if (tamano_destino == 32) {
        int32_t* registro_destino2 = registro_destino;
        *registro_destino2 += contenido_origen;
        printf("%d\n", *registro_destino2);
    }
}

void sub(char* nombre_registro_destino, char* nombre_registro_origen){
    int tamano_origen = 0;
    void* registro_origen = apuntar_a_registro(nombre_registro_origen, &tamano_origen);
    int contenido_origen = 0;

    if (tamano_origen == 8) {
        int8_t* registro_origen2 = registro_origen;
        contenido_origen = *registro_origen2;
        
    } else if (tamano_origen == 32) {
        int32_t* registro_origen2 = registro_origen;
        contenido_origen = *registro_origen2;
    }

    int tamano_destino = 0;
    void* registro_destino = apuntar_a_registro(nombre_registro_destino, &tamano_destino);

    if (tamano_destino == 8) {
        int8_t* registro_destino2 = registro_destino;
        *registro_destino2 -= contenido_origen;
        printf("%d\n", *registro_destino2);
    } else if (tamano_destino == 32) {
        int32_t* registro_destino2 = registro_destino;
        *registro_destino2 -= contenido_origen;
        printf("%d\n", *registro_destino2);
    }
}

void jnz(char* nombre_registro, int nuevo_pc){
    int tamano = 0;
    void* registro = apuntar_a_registro(nombre_registro, &tamano);
    int contenido = -1;
    
    if (tamano == 8) {
        int8_t* registro2 = registro;
        contenido = *registro2;
        
    } else if (tamano == 32) {
        int32_t* registro2 = registro;
        contenido = *registro2;
    }
    
    if(contenido){
        *los_registros_de_la_cpu -> PC = nuevo_pc -2;
    }
}

int resize(int tam){
    t_paquete* paquete = crear_paquete(RESIZE_CODE);
    agregar_int_a_paquete(paquete,pid_en_ejecucion);
    agregar_int_a_paquete(paquete,tam);
    enviar_paquete(paquete,cpu_cliente_memoria);
    eliminar_paquete(paquete);
    int ok = confirmacion_resize();
    return ok;
}

void mov_in(char* reg_datos, char* reg_direccion){ //GRAN PARTE DE LO QUE ESTA ACA DEBERIA PODER MODULARIZARCE EN FUNCIONES DENTRO DE LA MMU
    int tamano_datos = 0;
    void* registro_datos = apuntar_a_registro(reg_datos, &tamano_datos);

    tamano_datos = tamano_datos / 8;

    int tamano_direccion = 0;
    void* registro_direccion = apuntar_a_registro(reg_direccion, &tamano_direccion);

    direccion_fisica* dir_fisica;

    if (tamano_direccion == 8) {
        int8_t* registro_direccion2 = registro_direccion;
        dir_fisica = traducir_dir_logica(pid_en_ejecucion,*registro_direccion2);

    } else if (tamano_direccion == 32) {
        int32_t* registro_direccion2 = registro_direccion;
        dir_fisica = traducir_dir_logica(pid_en_ejecucion,*registro_direccion2);
    }

    if((tam_de_pags_memoria - dir_fisica ->desplazamiento) >= tamano_datos){
        t_paquete* paquete = crear_paquete(LECTURA_CODE);
        agregar_int_a_paquete(paquete, pid_en_ejecucion);
        agregar_int_a_paquete(paquete, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete, tamano_datos);

        enviar_paquete(paquete,cpu_cliente_memoria);
        eliminar_paquete(paquete);

        void* leido = recibir_lectura();

        memcpy(registro_datos,leido,tamano_datos);

        free(leido);
    }
    else{
        float tamano_datos_sobrantes = tamano_datos - (tam_de_pags_memoria - dir_fisica ->desplazamiento);

        int cant_nuevas_pags = ceil(tamano_datos_sobrantes / tam_de_pags_memoria);

        t_paquete* paquete2 = crear_paquete(LECTURA_CODE);
        agregar_int_a_paquete(paquete2, pid_en_ejecucion);
        agregar_int_a_paquete(paquete2, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete2, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        enviar_paquete(paquete2,cpu_cliente_memoria);
        eliminar_paquete(paquete2);

        void* leido1 = recibir_lectura();

        memcpy(registro_datos,leido1,(tam_de_pags_memoria - dir_fisica ->desplazamiento));

        free(leido1);

        int nueva_pag = dir_fisica ->num_de_pag_base + 1;

        int bytes_restantes_a_copiar = tamano_datos_sobrantes;

        int desplazamiento_en_reg_datos = (tam_de_pags_memoria - dir_fisica ->desplazamiento);

        for (int i = 0; i < cant_nuevas_pags; i++){
            int nuevo_marco = solicitar_marco(pid_en_ejecucion, nueva_pag);
            nueva_pag++;

            //Como tenemos que continuar desde la anterior pagina, el desplazamiento en el nuevo marco es 0, por lo que la direccion fisica es

            int nueva_dir_fisica_final = nuevo_marco * tam_de_pags_memoria;

            if(bytes_restantes_a_copiar > tam_de_pags_memoria){
                t_paquete* paquete3 = crear_paquete(LECTURA_CODE);
                agregar_int_a_paquete(paquete3, pid_en_ejecucion);
                agregar_int_a_paquete(paquete3, nueva_dir_fisica_final);
                agregar_int_a_paquete(paquete3, tam_de_pags_memoria);

                enviar_paquete(paquete3,cpu_cliente_memoria);
                eliminar_paquete(paquete3);

                void* leido2 = recibir_lectura();

                memcpy(registro_datos + desplazamiento_en_reg_datos, leido2, tam_de_pags_memoria);

                bytes_restantes_a_copiar = bytes_restantes_a_copiar - tam_de_pags_memoria;
                desplazamiento_en_reg_datos = desplazamiento_en_reg_datos + tam_de_pags_memoria;

                free(leido2);
            }
            else{
                t_paquete* paquete4 = crear_paquete(LECTURA_CODE);
                agregar_int_a_paquete(paquete4, pid_en_ejecucion);
                agregar_int_a_paquete(paquete4, nueva_dir_fisica_final);
                agregar_int_a_paquete(paquete4, bytes_restantes_a_copiar);

                enviar_paquete(paquete4,cpu_cliente_memoria);
                eliminar_paquete(paquete4);

                void* leido3 = recibir_lectura();

                memcpy(registro_datos + desplazamiento_en_reg_datos, leido3, bytes_restantes_a_copiar);

                free(leido3);
            }
        }

    }

    if(tamano_datos == 1){
        int8_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
    else if(tamano_datos == 4){
        int32_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
}

void mov_out(char* reg_direccion, char* reg_datos){
    int tamano_datos = 0;
    void* registro_datos = apuntar_a_registro(reg_datos, &tamano_datos);

    tamano_datos = tamano_datos / 8;

    int tamano_direccion = 0;
    void* registro_direccion = apuntar_a_registro(reg_direccion, &tamano_direccion);

    direccion_fisica* dir_fisica;

    if (tamano_direccion == 8) {
        int8_t* registro_direccion2 = registro_direccion;
        dir_fisica = traducir_dir_logica(pid_en_ejecucion,*registro_direccion2);

    } else if (tamano_direccion == 32) {
        int32_t* registro_direccion2 = registro_direccion;
        dir_fisica = traducir_dir_logica(pid_en_ejecucion,*registro_direccion2);
    }

    if((tam_de_pags_memoria - dir_fisica ->desplazamiento) >= tamano_datos){
        t_paquete* paquete = crear_paquete(ESCRITURA_CODE);
        agregar_int_a_paquete(paquete, pid_en_ejecucion);
        agregar_int_a_paquete(paquete, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete, tamano_datos);
        agregar_a_paquete(paquete, registro_datos, tamano_datos);

        enviar_paquete(paquete,cpu_cliente_memoria);
        eliminar_paquete(paquete);

    }
    else{
        float tamano_datos_sobrantes = tamano_datos - (tam_de_pags_memoria - dir_fisica ->desplazamiento);

        int cant_nuevas_pags = ceil(tamano_datos_sobrantes / tam_de_pags_memoria);

        t_paquete* paquete2 = crear_paquete(ESCRITURA_CODE);
        agregar_int_a_paquete(paquete2, pid_en_ejecucion);
        agregar_int_a_paquete(paquete2, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete2, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        void* fraccion1_reg_datos = malloc(tam_de_pags_memoria - dir_fisica ->desplazamiento);

        memcpy(fraccion1_reg_datos,registro_datos, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        agregar_a_paquete(paquete2,fraccion1_reg_datos,(tam_de_pags_memoria - dir_fisica ->desplazamiento));


        enviar_paquete(paquete2,cpu_cliente_memoria);
        eliminar_paquete(paquete2);

        free(fraccion1_reg_datos);


        int nueva_pag = dir_fisica ->num_de_pag_base + 1;

        int bytes_restantes_a_copiar = tamano_datos_sobrantes;

        int desplazamiento_en_reg_datos = (tam_de_pags_memoria - dir_fisica ->desplazamiento);

        for (int i = 0; i < cant_nuevas_pags; i++){
            int nuevo_marco = solicitar_marco(pid_en_ejecucion, nueva_pag);
            nueva_pag++;

            //Como tenemos que continuar desde la anterior pagina, el desplazamiento en el nuevo marco es 0, por lo que la direccion fisica es

            int nueva_dir_fisica_final = nuevo_marco * tam_de_pags_memoria;

            if(bytes_restantes_a_copiar > tam_de_pags_memoria){
                t_paquete* paquete3 = crear_paquete(ESCRITURA_CODE);
                agregar_int_a_paquete(paquete3, pid_en_ejecucion);
                agregar_int_a_paquete(paquete3, nueva_dir_fisica_final);
                agregar_int_a_paquete(paquete3, tam_de_pags_memoria);

                void* fraccionx_reg_datos = malloc(tam_de_pags_memoria);

                memcpy(fraccionx_reg_datos,registro_datos + desplazamiento_en_reg_datos, tam_de_pags_memoria);

                agregar_a_paquete(paquete3,fraccionx_reg_datos,tam_de_pags_memoria);


                enviar_paquete(paquete3,cpu_cliente_memoria);
                eliminar_paquete(paquete3);

                free(fraccionx_reg_datos);

                bytes_restantes_a_copiar = bytes_restantes_a_copiar - tam_de_pags_memoria;
                desplazamiento_en_reg_datos = desplazamiento_en_reg_datos + tam_de_pags_memoria;
            }
            else{
                t_paquete* paquete4 = crear_paquete(ESCRITURA_CODE);
                agregar_int_a_paquete(paquete4, pid_en_ejecucion);
                agregar_int_a_paquete(paquete4, nueva_dir_fisica_final);
                agregar_int_a_paquete(paquete4, bytes_restantes_a_copiar);

                void* fraccionX_reg_datos = malloc(bytes_restantes_a_copiar);

                memcpy(fraccionX_reg_datos,registro_datos + desplazamiento_en_reg_datos, bytes_restantes_a_copiar);

                agregar_a_paquete(paquete4,fraccionX_reg_datos, bytes_restantes_a_copiar);


                enviar_paquete(paquete4,cpu_cliente_memoria);
                eliminar_paquete(paquete4);
            }
        }

    }

    if(tamano_datos == 1){
        int8_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: ESCRIBIR - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
    else if(tamano_datos == 4){
        int32_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: ESCRIBIR - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
}

    void copy_string(int tamanio){
    int tamano_datos = 0;
    void* registroSI = apuntar_a_registro("SI", &tamano_datos);

   // char* contenido = *registroSI; //a chekear si esta bien esto
    //int tamanio_contenido = sizeof(*contenido);

    int tamanio_contenido = sizeof(*registroSI);

    tamano_datos = tamano_datos/8;

    int tamano_direccion = 0;
    void* registroDI = apuntar_a_registro("DI", &tamano_direccion);

    int32_t* registro_direccion2 = registroDI;
    direccion_fisica* dir_fisica = traducir_dir_logica(pid_en_ejecucion, *registro_direccion2);

    if((tam_de_pags_memoria - dir_fisica ->desplazamiento) >= tamanio){
        t_paquete* paquete = crear_paquete(ESCRITURA_CODE);
        agregar_int_a_paquete(paquete, pid_en_ejecucion);
        agregar_int_a_paquete(paquete, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete, tamanio);
        agregar_a_paquete(paquete, registroSI, tamanio_contenido);

        enviar_paquete(paquete,cpu_cliente_memoria);
        eliminar_paquete(paquete);

    }
    else{
        float tamano_datos_sobrantes = tamanio - (tam_de_pags_memoria - dir_fisica ->desplazamiento);

        int cant_nuevas_pags = ceil(tamano_datos_sobrantes / tam_de_pags_memoria);

        t_paquete* paquete2 = crear_paquete(ESCRITURA_CODE);
        agregar_int_a_paquete(paquete2, pid_en_ejecucion);
        agregar_int_a_paquete(paquete2, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete2, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        void* fraccion1_reg_datos = malloc(tam_de_pags_memoria - dir_fisica ->desplazamiento);

        memcpy(fraccion1_reg_datos,registroDI, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        agregar_a_paquete(paquete2,fraccion1_reg_datos,(tam_de_pags_memoria - dir_fisica ->desplazamiento));


        enviar_paquete(paquete2,cpu_cliente_memoria);
        eliminar_paquete(paquete2);

        free(fraccion1_reg_datos);


        int nueva_pag = dir_fisica ->num_de_pag_base + 1;

        int bytes_restantes_a_copiar = tamano_datos_sobrantes;

        int desplazamiento_en_reg_datos = (tam_de_pags_memoria - dir_fisica ->desplazamiento);

         for (int i = 0; i < cant_nuevas_pags; i++){
            int nuevo_marco = solicitar_marco(pid_en_ejecucion, nueva_pag);
            nueva_pag++;

            //Como tenemos que continuar desde la anterior pagina, el desplazamiento en el nuevo marco es 0, por lo que la direccion fisica es

            int nueva_dir_fisica_final = nuevo_marco * tam_de_pags_memoria;

            if(bytes_restantes_a_copiar > tam_de_pags_memoria){
                t_paquete* paquete3 = crear_paquete(ESCRITURA_CODE);
                agregar_int_a_paquete(paquete3, pid_en_ejecucion);
                agregar_int_a_paquete(paquete3, nueva_dir_fisica_final);
                agregar_int_a_paquete(paquete3, tam_de_pags_memoria);

                void* fraccionx_reg_datos = malloc(tam_de_pags_memoria);

                memcpy(fraccionx_reg_datos,registroDI + desplazamiento_en_reg_datos, tam_de_pags_memoria);

                agregar_a_paquete(paquete3,fraccionx_reg_datos,tam_de_pags_memoria);


                enviar_paquete(paquete3,cpu_cliente_memoria);
                eliminar_paquete(paquete3);

                free(fraccionx_reg_datos);

                bytes_restantes_a_copiar = bytes_restantes_a_copiar - tam_de_pags_memoria;
                desplazamiento_en_reg_datos = desplazamiento_en_reg_datos + tam_de_pags_memoria;
            }
            else{
                t_paquete* paquete4 = crear_paquete(ESCRITURA_CODE);
                agregar_int_a_paquete(paquete4, pid_en_ejecucion);
                agregar_int_a_paquete(paquete4, nueva_dir_fisica_final);
                agregar_int_a_paquete(paquete4, bytes_restantes_a_copiar);

                void* fraccionX_reg_datos = malloc(bytes_restantes_a_copiar);

                memcpy(fraccionX_reg_datos,registroDI + desplazamiento_en_reg_datos, bytes_restantes_a_copiar);

                agregar_a_paquete(paquete4,fraccionX_reg_datos, bytes_restantes_a_copiar);


                enviar_paquete(paquete4,cpu_cliente_memoria);
                eliminar_paquete(paquete4);
            }
        }

    }
    char* contenido = (char*)registroSI;
    log_info(logger_obligatorio, "PID: %d- Accion: ESCRIBIR - Dirección Fisica: %d - Valor: %.*s", pid_en_ejecucion, dir_fisica ->dir_fisica_final, tamanio, contenido);
    
}

int ejecutar_instruccion (int codigo_instruccion) {
    

    switch (codigo_instruccion)
    {
    case SET: // SET (Registro, Valor)
        int numero = atoi(instruccion_separada[2]);
        set(instruccion_separada[1], numero);
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], 
        instruccion_separada[2]);
        return SEGUIR_EJECUTANDO;

    case SUM: // SUM (Registro Destino, Registro Origen)
        sum(instruccion_separada[1], instruccion_separada[2]);
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], 
        instruccion_separada[2]);
        return SEGUIR_EJECUTANDO;

    case SUB: // SUB (Registro Destino, Registro Origen)
        sub(instruccion_separada[1], instruccion_separada[2]);
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], 
        instruccion_separada[2]);
        return SEGUIR_EJECUTANDO;

    case JNZ: // JNZ (Registro, Instrucción)
        int nuevo_pc = atoi(instruccion_separada[2]);
        jnz(instruccion_separada[1], nuevo_pc);
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], 
        instruccion_separada[2]);
        return SEGUIR_EJECUTANDO;

    case IO_GEN_SLEEP: // IO_GEN_SLEEP (Interfaz, Unidades de trabajo)
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], 
        instruccion_separada[2]);
        return SLEEP_GEN;

    case WAIT: // WAIT (RECURSO)
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1]);
        return WAIT_RECURSO;

    case SIGNAL: //SIGNAL (RECURSO)
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1]);
        return SIGNAL_RECURSO;

    case RESIZE: //RESIZE (TAMAÑO)
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1]);
        int tam_a_resize = atoi(instruccion_separada[1]);
        int ok = resize(tam_a_resize);
        if(ok){
            return SEGUIR_EJECUTANDO;
        }
        else{
            return SIN_MEMORIA;
        }

    case MOV_IN: //MOV IN (Registro Datos, Registro Dirección)
        mov_in(instruccion_separada[1],instruccion_separada[2]);
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], instruccion_separada[2]);
        return SEGUIR_EJECUTANDO;
    case MOV_OUT: 
        mov_out(instruccion_separada[1],instruccion_separada[2]);
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1], instruccion_separada[2]);
        return SEGUIR_EJECUTANDO;
    case COPY_STRING:
        int tamanio= atoi(instruccion_separada[1]);
        copy_string(tamanio);
        log_info(logger_obligatorio,"PID: %d - EJECUTANDO: %s %s - ",pid_en_ejecucion, instruccion_separada[0], instruccion_separada[1]);
        return SEGUIR_EJECUTANDO;
    case EXIT:
        log_info(logger_obligatorio, "PID: %d - EJECUTANDO: %s - ",pid_en_ejecucion, instruccion_separada[0]);
        return FINALIZAR;

    default:
        printf("Execute: Comando no reconocido");
        return 1;
    }

    return -1;
}

void* apuntar_a_registro (char* regist, int* puntero_a_tamano) {
    if (!strcmp(regist, "PC") || !strcmp(regist, "PC\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->PC;
        
    } else if (!strcmp(regist, "AX") || !strcmp(regist, "AX\n")) {
        *puntero_a_tamano = 8;
        return los_registros_de_la_cpu->AX;
    } else if (!strcmp(regist, "BX") || !strcmp(regist, "BX\n")) {
        *puntero_a_tamano = 8;
        return los_registros_de_la_cpu->BX;
    } else if (!strcmp(regist, "CX") || !strcmp(regist, "CX\n")) {
        *puntero_a_tamano = 8;
        return los_registros_de_la_cpu->CX;
    } else if (!strcmp(regist, "DX") || !strcmp(regist, "DX\n")) {
        *puntero_a_tamano = 8;
        return los_registros_de_la_cpu->DX;
    } else if (!strcmp(regist, "EAX") || !strcmp(regist, "EAX\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->EAX;
    } else if (!strcmp(regist, "EBX") || !strcmp(regist, "EBX\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->EBX;
    } else if (!strcmp(regist, "ECX") || !strcmp(regist, "ECX\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->ECX;
    } else if (!strcmp(regist, "EDX") || !strcmp(regist, "EDX\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->EDX;
    } else if (!strcmp(regist, "SI") || !strcmp(regist, "SI\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->SI;
    } else if (!strcmp(regist, "DI") || !strcmp(regist, "DI\n")) {
        *puntero_a_tamano = 32;
        return los_registros_de_la_cpu->DI;
    } else {
        log_error(logger, "Error al identificar");
        return NULL;
    }

    return NULL;
}

void cargar_registros_a_paquete(t_paquete* paquete){
    agregar_a_paquete(paquete,los_registros_de_la_cpu->PC,sizeof(uint32_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->AX,sizeof(uint8_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->BX,sizeof(uint8_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->CX,sizeof(uint8_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->DX,sizeof(uint8_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->EAX,sizeof(uint32_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->EBX,sizeof(uint32_t)); 
    agregar_a_paquete(paquete,los_registros_de_la_cpu->ECX,sizeof(uint32_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->EDX,sizeof(uint32_t));
    agregar_a_paquete(paquete,los_registros_de_la_cpu->SI,sizeof(uint32_t)); 
    agregar_a_paquete(paquete,los_registros_de_la_cpu->DI,sizeof(uint32_t));
}