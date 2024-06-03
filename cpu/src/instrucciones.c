#include <instrucciones.h>


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

        if(tamano_datos == 1){
            int8_t* para_log = leido;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }
        else if(tamano_datos == 4){
            int32_t* para_log = leido;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }

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

        if(tamano_datos == 1){
            int8_t* para_log = leido1;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }
        else if(tamano_datos == 4){
            int32_t* para_log = leido1;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }

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

                if(tamano_datos == 1){
                    int8_t* para_log = leido2;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }
                else if(tamano_datos == 4){
                    int32_t* para_log = leido2;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }

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

                if(tamano_datos == 1){
                    int8_t* para_log = leido3;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }
                else if(tamano_datos == 4){
                    int32_t* para_log = leido3;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }

                memcpy(registro_datos + desplazamiento_en_reg_datos, leido3, bytes_restantes_a_copiar);

                free(leido3);
            }
        }

    }
    /*
    if(tamano_datos == 1){
        int8_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
    else if(tamano_datos == 4){
        int32_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
    */
}

bool mov_out(char* reg_direccion, char* reg_datos){
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

        bool confirm = confirmacion_escritura();

        if(!confirm){
            return false;
        }

        if(tamano_datos == 1){
            int8_t* para_log = registro_datos;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }
        else if(tamano_datos == 4){
            int32_t* para_log = registro_datos;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }

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

        bool confirm = confirmacion_escritura();

        if(!confirm){
            free(fraccion1_reg_datos);
            return false;
        }

        if(tamano_datos == 1){
            int8_t* para_log = fraccion1_reg_datos;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }
        else if(tamano_datos == 4){
            int32_t* para_log = fraccion1_reg_datos;
            log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *para_log);
        }

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

                bool confirm = confirmacion_escritura();

                if(!confirm){
                    free(fraccionx_reg_datos);
                    return false;
                }


                if(tamano_datos == 1){
                    int8_t* para_log = fraccionx_reg_datos;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }
                else if(tamano_datos == 4){
                    int32_t* para_log = fraccionx_reg_datos;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }

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


                bool confirm = confirmacion_escritura();

                if(!confirm){
                    free(fraccionX_reg_datos);
                    return false;
                }


                if(tamano_datos == 1){
                    int8_t* para_log = fraccionX_reg_datos;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }
                else if(tamano_datos == 4){
                    int32_t* para_log = fraccionX_reg_datos;
                    log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, nueva_dir_fisica_final, *para_log);
                }

                free(fraccionX_reg_datos);
            }
        }

    }

    /*
    if(tamano_datos == 1){
        int8_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: ESCRIBIR - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
    else if(tamano_datos == 4){
        int32_t* registro_datos2 = registro_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: ESCRIBIR - Dirección Fisica: %d - Valor: %d", pid_en_ejecucion, dir_fisica ->dir_fisica_final, *registro_datos2);
    }
    */

   return true;
}

bool copy_string(int tamanio){
    int tamano_datos = 0;
    void* registroSI = apuntar_a_registro("SI", &tamano_datos);

   // char* contenido = *registroSI; //a chekear si esta bien esto
    //int tamanio_contenido = sizeof(*contenido);

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
        agregar_a_paquete(paquete, registroSI, tamanio);

        enviar_paquete(paquete,cpu_cliente_memoria);
        eliminar_paquete(paquete);

        bool confirm = confirmacion_escritura();

        if(!confirm){
            return false;
        }

        char* para_log = malloc(tamanio + 1);
        memcpy((void*)para_log, registroSI, tamanio);
        log_info(logger_obligatorio, "PID: %d- Acción: ESCRIBIR - Dirección Fisica: %d - Valor: %s", pid_en_ejecucion, dir_fisica ->dir_fisica_final, para_log);

        free(para_log);
    
    }
    else{
        float tamano_datos_sobrantes = tamanio - (tam_de_pags_memoria - dir_fisica ->desplazamiento);

        int cant_nuevas_pags = ceil(tamano_datos_sobrantes / tam_de_pags_memoria);

        t_paquete* paquete2 = crear_paquete(ESCRITURA_CODE);
        agregar_int_a_paquete(paquete2, pid_en_ejecucion);
        agregar_int_a_paquete(paquete2, dir_fisica ->dir_fisica_final);
        agregar_int_a_paquete(paquete2, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        void* fraccion1_reg_datos = malloc(tam_de_pags_memoria - dir_fisica ->desplazamiento);

        memcpy(fraccion1_reg_datos,registroSI, (tam_de_pags_memoria - dir_fisica ->desplazamiento));

        agregar_a_paquete(paquete2,fraccion1_reg_datos,(tam_de_pags_memoria - dir_fisica ->desplazamiento));


        enviar_paquete(paquete2,cpu_cliente_memoria);
        eliminar_paquete(paquete2);

        

        bool confirm = confirmacion_escritura();

        if(!confirm){
            free(fraccion1_reg_datos);
            return false;
        }

        char* para_log = fraccion1_reg_datos;
        log_info(logger_obligatorio, "PID: %d- Acción: ESCRIBIR - Dirección Fisica: %d - Valor: %s", pid_en_ejecucion, dir_fisica ->dir_fisica_final, para_log);

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

                memcpy(fraccionx_reg_datos,registroSI + desplazamiento_en_reg_datos, tam_de_pags_memoria);

                agregar_a_paquete(paquete3,fraccionx_reg_datos,tam_de_pags_memoria);


                enviar_paquete(paquete3,cpu_cliente_memoria);
                eliminar_paquete(paquete3);

                bool confirm = confirmacion_escritura();

                if(!confirm){
                    free(fraccionx_reg_datos);
                    return false;
                }

                char* para_log = fraccionx_reg_datos;
                log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %s", pid_en_ejecucion, nueva_dir_fisica_final, para_log);

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

                memcpy(fraccionX_reg_datos,registroSI + desplazamiento_en_reg_datos, bytes_restantes_a_copiar);

                agregar_a_paquete(paquete4,fraccionX_reg_datos, bytes_restantes_a_copiar);


                enviar_paquete(paquete4,cpu_cliente_memoria);
                eliminar_paquete(paquete4);

                bool confirm = confirmacion_escritura();

                if(!confirm){
                    free(fraccionX_reg_datos);
                    return false;
                }

                char* para_log = fraccionX_reg_datos;
                log_info(logger_obligatorio, "PID: %d- Acción: LEER - Dirección Fisica: %d - Valor: %s", pid_en_ejecucion, nueva_dir_fisica_final, para_log);

                free(fraccionX_reg_datos);
            }
        }

    }
    /*
    char* contenido = (char*)registroSI;
    log_info(logger_obligatorio, "PID: %d- Accion: ESCRIBIR - Dirección Fisica: %d - Valor: %.*s", pid_en_ejecucion, dir_fisica ->dir_fisica_final, tamanio, contenido);
    */

   return true;
    
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