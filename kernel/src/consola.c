#include <consola.h>

void consola_kernel(){
    
    log_info(logger, "INICIANDO CONSOLA \n");
    char* leido;
    leido = readline(">");

    while(strcmp(leido,"EXIT")!=0){
        add_history(leido);
        char** comando_por_partes = string_split(leido, " "); //Se divide lo leido, separandolos por los espacios
        validar_y_ejecutar_comando(comando_por_partes);
        string_array_destroy(comando_por_partes);
        free(leido);
        leido = readline(">");
    }
    free(leido);
}

void validar_y_ejecutar_comando(char** comando_por_partes){
    
    //En cuanto se agreguen las funciones de los comandos, hay que agregar mas validaciones
    
    if(strcmp(comando_por_partes[0],"EJECUTAR_SCRIPT")==0 && (string_array_size(comando_por_partes)==2) && (strcmp(comando_por_partes[1],"")!=0)){
        
        ejecutar_script(comando_por_partes[1]);
    }
    else if((strcmp(comando_por_partes[0],"INICIAR_PROCESO")==0) && (string_array_size(comando_por_partes)==2) && (strcmp(comando_por_partes[1],"")!=0)){
        pthread_t hilo_crear_proceso;
        char* ruta = strdup(comando_por_partes[1]);
        pthread_create(&hilo_crear_proceso, NULL, (void*)crear_proceso,(void*)ruta);
        pthread_detach(hilo_crear_proceso);
    }
    else if(strcmp(comando_por_partes[0],"FINALIZAR_PROCESO")==0 && (string_array_size(comando_por_partes)==1)){
        printf("Hola, soy finalizar proceso\n");
        
    }
    else if(strcmp(comando_por_partes[0],"DETENER_PLANIFICACION")==0 && (string_array_size(comando_por_partes)==1)){
        permitir_planificacion = false;
    }
    else if(strcmp(comando_por_partes[0],"INICIAR_PLANIFICACION")==0 && (string_array_size(comando_por_partes)==1)){
        
        iniciar_planificacion();
    }
    else if(strcmp(comando_por_partes[0],"MULTIPROGRAMACION")==0 && (string_array_size(comando_por_partes)==2) && (strcmp(comando_por_partes[1],"")!=0)){
        int nuevo_grado_multi = atoi(comando_por_partes[1]);
        int diferencia = nuevo_grado_multi - GRADO_MULTIPROGRAMACION;
        if(diferencia > 0){
            for(int i = 0; i< diferencia; i++){
                sem_post(&(multiprogramacion_permite_proceso_en_ready));
            }
        }
        else if(diferencia <0){
            espera_grado_multi = (-1)*diferencia;
        }
        GRADO_MULTIPROGRAMACION = nuevo_grado_multi;
    }
    else if(strcmp(comando_por_partes[0],"PROCESO_ESTADO")==0 && (string_array_size(comando_por_partes)==1)){
        printf("Hola, soy proceso estado\n");
        pcb* pcb_revisar;
        int largo_cola;
        printf("COLA NEW: \n");

        pthread_mutex_lock(&mutex_cola_new);
        largo_cola = queue_size(cola_new);
        pthread_mutex_unlock(&mutex_cola_new);

        for(int i = 0; i< largo_cola; i++){

            pthread_mutex_lock(&mutex_cola_new);
            pcb_revisar = list_get(cola_new->elements,i);
            pthread_mutex_unlock(&mutex_cola_new);

            printf("%d\n",pcb_revisar ->PID);
        }
        printf("COLA READY: \n");

        pthread_mutex_lock(&mutex_cola_ready);
        largo_cola = queue_size(cola_ready);
        pthread_mutex_unlock(&mutex_cola_ready);

        for(int i = 0; i< largo_cola; i++){

            pthread_mutex_lock(&mutex_cola_ready);
            pcb_revisar = list_get(cola_ready->elements,i);
            pthread_mutex_unlock(&mutex_cola_ready);

            printf("%d\n",pcb_revisar ->PID);
        }

        if(strcmp(ALGORITMO_PLANIFICACION, "vrr")==0){
            printf("COLA READY PRIORITARIA: \n");

            pthread_mutex_lock(&mutex_cola_prioritaria);
            largo_cola = queue_size(cola_ready_prioritaria);
            pthread_mutex_unlock(&mutex_cola_prioritaria);

            for(int i = 0; i< largo_cola; i++){

                pthread_mutex_lock(&mutex_cola_prioritaria);
                pcb_revisar = list_get(cola_ready_prioritaria->elements,i);
                pthread_mutex_unlock(&mutex_cola_prioritaria);

                printf("%d\n",pcb_revisar ->PID);
            }
        }
        printf("COLA EXEC: \n");
        if(proceso_en_ejecucion != NULL){
            printf("%d\n",proceso_en_ejecucion ->PID);
        }
        else{
            printf("\n");
        }
        printf("COLAS BLOCKED: \n");
        bool diccionario_vacio;

        pthread_mutex_lock(&mutex_para_diccionario_blocked);
        diccionario_vacio = dictionary_is_empty(diccionario_blocked);
        pthread_mutex_unlock(&mutex_para_diccionario_blocked);

        if(!diccionario_vacio){
            nodo_de_diccionario_blocked* nodo;

            pthread_mutex_lock(&mutex_para_diccionario_blocked);
            t_list* lista_keys = dictionary_keys(diccionario_blocked);
            pthread_mutex_unlock(&mutex_para_diccionario_blocked);

            for(int i = 0; i<list_size(lista_keys); i++){
                char* interfaz_o_recurso = list_get(lista_keys,i);

                pthread_mutex_lock(&mutex_para_diccionario_blocked);
                nodo = dictionary_get(diccionario_blocked,interfaz_o_recurso);
                pthread_mutex_unlock(&mutex_para_diccionario_blocked);

                printf("COLA BLOCKED DE %s",interfaz_o_recurso);

                pthread_mutex_lock(&(nodo ->mutex_para_cola_bloqueados));
                largo_cola = queue_size(nodo ->cola_bloqueados);
                pthread_mutex_unlock(&(nodo ->mutex_para_cola_bloqueados));

                for(int p = 0; p<largo_cola; p++){

                    pthread_mutex_lock(&(nodo ->mutex_para_cola_bloqueados));
                    pcb_revisar = list_get(nodo ->cola_bloqueados ->elements,p);
                    pthread_mutex_unlock(&(nodo ->mutex_para_cola_bloqueados));

                    printf("%d\n",pcb_revisar ->PID);
                }

            }
            list_destroy(lista_keys);
        }

        pthread_mutex_lock(&mutex_para_diccionario_recursos);
        diccionario_vacio = dictionary_is_empty(diccionario_recursos);
        pthread_mutex_unlock(&mutex_para_diccionario_recursos);

        if(!diccionario_vacio){
            nodo_recursos* nodo_del_recurso;

            pthread_mutex_lock(&mutex_para_diccionario_recursos);
            t_list* lista_keys_recurso = dictionary_keys(diccionario_recursos);
            pthread_mutex_unlock(&mutex_para_diccionario_recursos);

            for(int i = 0; i<list_size(lista_keys_recurso); i++){
                char* recurso = list_get(lista_keys_recurso,i);

                pthread_mutex_lock(&mutex_para_diccionario_recursos);
                nodo_del_recurso = dictionary_get(diccionario_recursos,recurso);
                pthread_mutex_unlock(&mutex_para_diccionario_recursos);

                printf("COLA BLOCKED DE %s",recurso);

                pthread_mutex_lock(&(nodo_del_recurso ->mutex_del_recurso));
                largo_cola = queue_size(nodo_del_recurso ->cola_bloqueados_recurso);
                

                for(int p = 0; p<largo_cola; p++){
                    pcb_revisar = list_get(nodo_del_recurso ->cola_bloqueados_recurso ->elements,p);

                    printf("%d\n",pcb_revisar ->PID);
                }
                pthread_mutex_unlock(&(nodo_del_recurso ->mutex_del_recurso));
            }
            list_destroy(lista_keys_recurso);
        }

        printf("COLA EXIT: \n");

        pthread_mutex_lock(&mutex_cola_exit);
        largo_cola = queue_size(cola_exit);
        pthread_mutex_unlock(&mutex_cola_exit);

        for(int i = 0; i< largo_cola; i++){

            pthread_mutex_lock(&mutex_cola_exit);
            pcb_revisar = list_get(cola_exit->elements,i);
            pthread_mutex_unlock(&mutex_cola_exit);

            printf("%d\n",pcb_revisar ->PID);
        }
        
    }
    else{
        log_error(logger, "ERROR. COMANDO NO RECONOCIDO O SINTAXIS ERRONEA");
    }
    
}

void crear_proceso(void* ruta_pseudocodigo){
    // Recibira un path por consola.
    // Crear PCB del proceso.
    pthread_mutex_lock(&mutex_para_creacion_proceso);
    pcb* pcb_proceso = creacion_pcb((char*)ruta_pseudocodigo);
    if(pcb_proceso != NULL){
        pthread_mutex_lock(&mutex_cola_new);
        queue_push(cola_new,pcb_proceso);
        pthread_mutex_unlock(&mutex_cola_new);
        
        sem_post(&hay_proceso_en_new);

    }
    else{
        log_error(logger,"No existe ese archivo de Pseudocodigo");
    }
    free(ruta_pseudocodigo);

    pthread_mutex_unlock(&mutex_para_creacion_proceso);
}

void iniciar_planificacion(){
    if(!permitir_planificacion){
        sem_post(&detener_planificacion_corto_plazo);
        sem_post(&detener_planificacion_exit);
        sem_post(&detener_planificacion_salida_cpu);
        sem_post(&detener_planificacion_to_ready);
        bool diccionario_vacio;

        pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
        diccionario_vacio = dictionary_is_empty(diccionario_entrada_salida);
        pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

        if(!diccionario_vacio){
            nodo_de_diccionario_interfaz* nodo;

            pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
            t_list* lista_keys = dictionary_keys(diccionario_entrada_salida);
            pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

            for(int i = 0; i<list_size(lista_keys); i++){
                char* interfaz_o_recurso = list_get(lista_keys,i);

                pthread_mutex_lock(&mutex_para_diccionario_entradasalida);
                nodo = dictionary_get(diccionario_entrada_salida,interfaz_o_recurso);
                pthread_mutex_unlock(&mutex_para_diccionario_entradasalida);

                sem_post(&(nodo ->detener_planificacion_enviar_peticion_IO));
                sem_post(&(nodo ->detener_planificacion_recibir_respuestas_IO));

            }
        }
        permitir_planificacion = true;
    }
}

void ejecutar_script(char* nombre_archivo){
    char* archivo = strdup(PATH_SCRIPTS);
    string_append(&archivo, nombre_archivo);

    FILE* archivo_script = fopen(archivo, "r");

    if(archivo_script != NULL){
        while(!feof(archivo_script)){
            usleep(500 * 1000);
            char instruccion_consola[256];
            fgets(instruccion_consola,256,archivo_script);

            if(instruccion_consola[strlen(instruccion_consola)-1] == '\n'){
                instruccion_consola[strlen(instruccion_consola)-1] = '\0';
            }
            

            char** comando_por_partes = string_split(instruccion_consola, " ");

            validar_y_ejecutar_comando(comando_por_partes);

            string_array_destroy(comando_por_partes);

        }

    }
    else{
        log_error(logger,"No existe ese archivo de Script");
    }

    fclose(archivo_script);
    free(archivo);
}