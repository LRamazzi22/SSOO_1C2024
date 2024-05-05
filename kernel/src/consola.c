#include <consola.h>

void consola_kernel(){
    
    log_info(logger, "INICIANDO CONSOLA \n");
    char* leido;
    leido = readline(">");

    while(strcmp(leido,"EXIT")!=0){
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
    
    if(strcmp(comando_por_partes[0],"EJECUTAR_SCRIPT")==0){
        printf("Hola, soy ejecutar script\n");
    }
    else if((strcmp(comando_por_partes[0],"INICIAR_PROCESO")==0) && (string_array_size(comando_por_partes)==2) && (strcmp(comando_por_partes[1],"")!=0)){
        pthread_t hilo_crear_proceso;
        char* ruta = strdup(comando_por_partes[1]);
        pthread_create(&hilo_crear_proceso, NULL, (void*)crear_proceso,(void*)ruta);
        pthread_detach(hilo_crear_proceso);
    }
    else if(strcmp(comando_por_partes[0],"FINALIZAR_PROCESO")==0){
        printf("Hola, soy finalizar proceso\n");
        
    }
    else if(strcmp(comando_por_partes[0],"DETENER_PLANIFICACION")==0){
        permitir_planificacion = false;
    }
    else if(strcmp(comando_por_partes[0],"INICIAR_PLANIFICACION")==0){
        permitir_planificacion = true;
    }
    else if(strcmp(comando_por_partes[0],"MULTIPROGRAMACION")==0){
        printf("Hola, soy multiprogramacion\n");
    }
    else if(strcmp(comando_por_partes[0],"PROCESO_ESTADO")==0){
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
            t_list* lista_keys = dictionary_keys(diccionario_blocked);
            for(int i = 0; i<list_size(lista_keys); i++){
                char* interfaz_o_recurso = list_get(lista_keys,i);

                pthread_mutex_lock(&mutex_para_diccionario_blocked);
                nodo = dictionary_get(diccionario_blocked,interfaz_o_recurso);
                pthread_mutex_unlock(&mutex_para_diccionario_blocked);

                printf("COLA BLOCKED DE %s",interfaz_o_recurso);

                pthread_mutex_lock(&(nodo ->mutex_para_cola));
                largo_cola = queue_size(nodo ->cola_bloqueados);
                pthread_mutex_unlock(&(nodo ->mutex_para_cola));

                for(int p = 0; p<largo_cola; p++){

                    pthread_mutex_lock(&(nodo ->mutex_para_cola));
                    pcb_revisar = list_get(nodo ->cola_bloqueados ->elements,p);
                    pthread_mutex_unlock(&(nodo ->mutex_para_cola));

                    printf("%d\n",pcb_revisar ->PID);
                }

            }
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