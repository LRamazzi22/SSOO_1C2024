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