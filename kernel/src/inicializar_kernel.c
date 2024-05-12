#include <inicializar_kernel.h>

void inicializar_kernel(){
    inicializar_config_kernel();

    pid_acumulado = 0;
    pid_a_eliminar = -1;
    espera_grado_multi = -1;
    cantidad_de_proceso_en_ejecucion = GRADO_MULTIPROGRAMACION;
    permitir_planificacion = true;

    logger = iniciar_logger("./kernel.log", "Kernel_Logger", LOG_LEVEL_INFO);
    logger_obligatorio = iniciar_logger("./kernelObligatorio.log", "Kernel_Logger_Obligatorio", LOG_LEVEL_INFO);
    cola_new = queue_create();
    cola_ready = queue_create();
    diccionario_blocked = dictionary_create();
    cola_exit = queue_create();

    diccionario_entrada_salida = dictionary_create();
    diccionario_recursos = dictionary_create();

    //Semaforos
    sem_init(&hay_proceso_en_ready,0,0);
    sem_init(&hay_proceso_en_new,0,0);
    sem_init(&hay_proceso_en_exit,0,0);
    sem_init(&multiprogramacion_permite_proceso_en_ready,0,GRADO_MULTIPROGRAMACION);

    sem_init(&detener_planificacion_corto_plazo,0,0);
    sem_init(&detener_planificacion_exit,0,0);
    sem_init(&detener_planificacion_salida_cpu,0,0);
    sem_init(&detener_planificacion_to_ready,0,0);

    inicializar_recursos();
    
}

void inicializar_config_kernel(){
    config = iniciar_config("./kernel_config.config");

    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    IP_CPU = config_get_string_value(config,"IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    ALGORITMO_PLANIFICACION = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
    QUANTUM = config_get_int_value(config, "QUANTUM");
    RECURSOS = config_get_array_value(config,"RECURSOS");
    INSTANCIAS_RECURSOS = config_get_array_value(config,"INSTANCIAS_RECURSOS");
    GRADO_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
}

void inicializar_recursos(){
    if(!string_array_is_empty(RECURSOS)){
        for(int i = 0; i< string_array_size(RECURSOS); i++){
            nodo_recursos* nodo = malloc(sizeof(nodo_recursos));
            nodo ->cola_bloqueados_recurso = queue_create();
            nodo ->instancias = atoi(INSTANCIAS_RECURSOS[i]);

            pthread_mutex_lock(&mutex_para_diccionario_recursos);
            dictionary_put(diccionario_recursos,RECURSOS[i], nodo);
            pthread_mutex_unlock(&mutex_para_diccionario_recursos);
            
        }
    }
}
