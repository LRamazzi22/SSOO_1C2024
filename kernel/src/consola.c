#include <consola.h>

void consola_kernel(){
    
    log_info(logger, "INICIANDO CONSOLA \n");
    char* leido;
    leido = readline(">");
    int tipo_comando;

    while(strcmp(leido,"EXIT")!=0){
        char** comando_por_partes = string_split(leido, " "); //SE divide lo leido, separandolos por los espacios
        tipo_comando = validar_comando(comando_por_partes); //DEvuelve un enum de comandos
        ejecutar_comando(tipo_comando, comando_por_partes); //Utilizando el enum y el comando ya separado por espacios, ejecuta el comando. EN caso de no existir, da el aviso de que no existe
        string_array_destroy(comando_por_partes);
        free(leido);
        leido = readline(">");
    }
    free(leido);
}

comandos validar_comando(char** comando_por_partes){
    
    //En cuanto se agreguen las funciones de los comandos, hay que agregar mas validaciones
    
    if(strcmp(comando_por_partes[0],"EJECUTAR_SCRIPT")==0){
        return EJECUTAR_SCRIPT;
    }
    else if(strcmp(comando_por_partes[0],"INICIAR_PROCESO")==0){
        return INICIAR_PROCESO;
    }
    else if(strcmp(comando_por_partes[0],"FINALIZAR_PROCESO")==0){
        return FINALIZAR_PROCESO;
    }
    else if(strcmp(comando_por_partes[0],"DETENER_PLANIFICACION")==0){
        return DETENER_PLANIFICACION;
    }
    else if(strcmp(comando_por_partes[0],"INICIAR_PLANIFICACION")==0){
        return INICIAR_PLANIFICACION;
    }
    else if(strcmp(comando_por_partes[0],"MULTIPROGRAMACION")==0){
        return MULTIPROGRAMACION;
    }
    else if(strcmp(comando_por_partes[0],"PROCESO_ESTADO")==0){
        return PROCESO_ESTADO;
    }
    else{
        return NO_RECONOCIDO;
    }
    

}

void ejecutar_comando(comandos tipo_comandos, char** comando_por_partes){

    switch (tipo_comandos)
    {
    case EJECUTAR_SCRIPT:
        printf("Hola, soy ejecutar script\n");
        break;
    
    case INICIAR_PROCESO:
        printf("Hola, soy iniciar proceso\n");
        break;

    case FINALIZAR_PROCESO:
        printf("Hola, soy finalizar proceso\n");
        break;

    case DETENER_PLANIFICACION:
        printf("Hola, soy detener planificacion\n");
        break;

    case INICIAR_PLANIFICACION:
        printf("Hola, soy iniciar planificacion\n");
        break;

    case MULTIPROGRAMACION:
        printf("Hola, soy multiprogramacion\n");
        break;

    case PROCESO_ESTADO:
        printf("Hola, soy proceso estado\n");
        break;

    case NO_RECONOCIDO:
        log_error(logger, "ERROR. COMANDO NO RECONOCIDO");
        break;
    default:
        log_error(logger, "ERROR. Que extraño, validar comando no detecto esto como comando no reconocido");
        break;
    }

}

int crear_proceso(char* ruta_pseudocodigo){
    
    int pid_acumulado;

    // Recibirá un path por consola.
    // Crear PCB del proceso.

    pcb *pcb_nuevo_proceso = malloc(sizeof(pcb)); // Liberar cuando no se use mas. (EXIT)

    // + Actualizar PCB.
    pcb_nuevo_proceso->estado_proceso = NEW;
    pcb_nuevo_proceso->quantum = QUANTUM; // Por el momento, después definimos el valor inicial.
    pcb_nuevo_proceso->PID = pid_acumulado++;

     // TODO: Mandarlo a la cola NEW 

    // Avisará a la memoria que debe iniciar un proceso.
     // La idea es pasarle el path a memoria, el FS se encuentra donde se ejecute el modulo de memoria. S
    // Mandar el OPCODE de iniciar proceso + buffer que contenga el path.

    t_paquete* paquete_codeop_ruta = crear_paquete(CREAR_PROCESO);
    agregar_string_a_paquete(paquete_codeop_ruta,ruta_pseudocodigo);
    enviar_paquete(paquete_codeop_ruta,kernel_cliente_memoria);
    eliminar_paquete(paquete_codeop_ruta);
   
    //SI EL GRADO DE MULTIPROG ACEPTA... CONTINUARÁ... EN otra función que se ocupe de mandar a READY.

    // Memoria me tiene que avisar que está tiene cargadas las instrucciones en memoria.
    // Cuando estén las instrucciones cargadas se actualizará el PCB con estado ready
    // Se pasará a la cola de READY + Actualizar PCB. De lo contrario seguirá en NEW hasta que aplique.

    return 0;
}