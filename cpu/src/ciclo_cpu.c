#include <ciclo_cpu.h>

void ciclo(){
    int terminar = 1;
    while(terminar){
        solicitar_instruccion(*los_registros_de_la_cpu -> PC);
        int cod_instruccion = decodificar_instruccion();
        terminar = ejecutar_instruccion(cod_instruccion);
        uint32_t* programCounter = los_registros_de_la_cpu -> PC;
        *programCounter++;
    }
    
}

void solicitar_instruccion(int programCounter){
    t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION);
    agregar_int_a_paquete(paquete, programCounter);
    enviar_paquete(paquete, cpu_cliente_memoria);
    eliminar_paquete(paquete);
    atender_memoria_cpu_sin_while();
    printf("%s",instruccion_a_decodificar);
}

int decodificar_instruccion(){
    // instruccion_a_decodificar
    char** instruccion_separada = string_split(instruccion_a_decodificar, " ");

    if (!strcmp(instruccion_separada[0],"SET") && lenght(instruccion_separada) == 3) {
        return SET;
        
    } else if (!strcmp(instruccion_separada[0],"MOV_IN")) {
        return MOV_IN;
        
    } else if (!strcmp(instruccion_separada[0], "MOV_OUT")) {
        return MOV_OUT;
        
    } else if (!strcmp(instruccion_separada[0], "SUM") && lenght(instruccion_separada) == 3) {
        return SUM;

    } else if (!strcmp(instruccion_separada[0], "SUB") && lenght(instruccion_separada) == 3) {
        return SUB;

    } else if (!strcmp(instruccion_separada[0], "JNZ") && lenght(instruccion_separada) == 3) {
        return JNZ;

    } else if (!strcmp(instruccion_separada[0], "RESIZE")) {
        return RESIZE;
        
    } else if (!strcmp(instruccion_separada[0], "COPY_STRING ")) {
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

void set(char * nombre_registro, int num){
    int tamano = apuntar_a_registro(nombre_registro, void* registro)
    if (tamano == 8) {
        int8_t* registro2 = registro;
        *registro2 = num;
    }
    if(tamano == 32)
    
    if (!strcmp(regist, "PC")) {
        *los_registros_de_la_cpu ->PC = num;
    } else if (!strcmp(regist, "AX")) {
        *los_registros_de_la_cpu->AX = num;
    } else if (!strcmp(regist, "BX")) {
        *los_registros_de_la_cpu->BX = num;
    } else if (!strcmp(regist, "CX")) {
        *los_registros_de_la_cpu->CX = num;
    } else if (!strcmp(regist, "DX")) {
        *los_registros_de_la_cpu->DX = num;
    } else if (!strcmp(regist, "EAX")) {
       *los_registros_de_la_cpu->EAX = num;
    } else if (!strcmp(regist, "EBX")) {
        *los_registros_de_la_cpu->EBX = num;
    } else if (!strcmp(regist, "ECX")) {
        *los_registros_de_la_cpu->ECX = num;
    } else if (!strcmp(regist, "EDX")) {
        *los_registros_de_la_cpu->EDX = num;
    } else if (!strcmp(regist, "SI")) {
        *los_registros_de_la_cpu->SI = num;
    } else if (!strcmp(regist, "DI")) {
        *los_registros_de_la_cpu->DI = num;
    } else {
        log_error(logger, "Error al identificar");
        return 0;
    }
}

int ejecutar_instruccion (int codigo_instruccion) {
    char** instruccion_separada = string_split(instruccion_a_decodificar, " ");
    switch (codigo_instruccion)
    {
    case SET: // SET (Registro, Valor)
        int numero = atoi(instruccion_separada[2]);
        set(instruccion_separada[1],numero);
        break;
    case SUM: // SUM (Registro Destino, Registro Origen)
        void* registroDestino;
        int tamDestino = apuntar_a_registro(instruccion_separada[1], registroDestino);
        void* registroOrigen;
        int tamDestino = apuntar_a_registro(instruccion_separada[2], registroOrigen);
        *registroDestino += *registroOrigen;
        return 1;
        break;
    case SUB: // SUB (Registro Destino, Registro Origen)
        void* registroDestino = apuntar_a_registro(instruccion_separada[1]);
        void* registroOrigen = apuntar_a_registro(instruccion_separada[2]);
        *registroDestino -= *registroOrigen;
        return 1;
        break;
    case JNZ: // JNZ (Registro, Instrucción)
        void* registroApuntado = apuntar_a_registro(instruccion_separada[1]);
        int nroInstruccion = instruccion_separada[2];
        if (*registroApuntado) {*los_registros_de_la_cpu->PC = nroInstruccion-1;} // Despues se hace ++
        return 1;
        break;
    case IO_GEN_SLEEP: // IO_GEN_SLEEP (Interfaz, Unidades de trabajo)
        printf("DORMITE");
        return 1;
        break;
    case EXIT:
        return 0;
    default:
        printf("Execute: Comando no reconocido");
        return 1;
    }
}

int apuntar_a_registro (char* regist, void* registro) {
    if (!strcmp(regist, "PC")) {
        return los_registros_de_la_cpu->PC;

    } else if (!strcmp(regist, "AX")) {
        return los_registros_de_la_cpu->AX;
    } else if (!strcmp(regist, "BX")) {
        return los_registros_de_la_cpu->BX;
    } else if (!strcmp(regist, "CX")) {
        return los_registros_de_la_cpu->CX;
    } else if (!strcmp(regist, "DX")) {
        return los_registros_de_la_cpu->DX;
    } else if (!strcmp(regist, "EAX")) {
        return los_registros_de_la_cpu->EAX;
    } else if (!strcmp(regist, "EBX")) {
        puntero_a_registro = los_registros_de_la_cpu->EBX;
        return 32;
    } else if (!strcmp(regist, "ECX")) {
        puntero_a_registro = los_registros_de_la_cpu->ECX;
        return 32;
    } else if (!strcmp(regist, "EDX")) {
        puntero_a_registro = los_registros_de_la_cpu->EDX;
        return 32;
    } else if (!strcmp(regist, "SI")) {
        puntero_a_registro = los_registros_de_la_cpu->SI;
        return 32;
    } else if (!strcmp(regist, "DI")) {
        puntero_a_registro = los_registros_de_la_cpu->DI;
        return 32;
    } else {
        log_error(logger, "Error al identificar");
        return 0;
    }
}