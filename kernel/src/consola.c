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
        printf("COLA NEW: \n");
        for(int i = 0; i< queue_size(cola_new); i++){
            pcb_revisar = list_get(cola_new->elements,i);
            printf("%d\n",pcb_revisar ->PID);
        }
        printf("COLA READY: \n");
        for(int i = 0; i< queue_size(cola_ready); i++){
            pcb_revisar = list_get(cola_ready->elements,i);
            printf("%d\n",pcb_revisar ->PID);
        }
        printf("COLA EXEC: \n");
        if(proceso_en_ejecucion != NULL){
            printf("%d\n",proceso_en_ejecucion ->PID);
        }
        else{
            printf("\n");
        }
        printf("COLA EXIT: \n");
        for(int i = 0; i< queue_size(cola_exit); i++){
            pcb_revisar = list_get(cola_exit->elements,i);
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