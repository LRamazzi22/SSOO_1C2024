#include <ciclo_cpu.h>
#include <atender_mensajes.h>

void ciclo(){
    int terminar = 1;
    while(terminar){
        solicitar_instruccion(*los_registros_de_la_cpu -> PC);
        int cod_instruccion = decodificar_instruccion();
        terminar = ejecutar_instruccion(cod_instruccion);
        uint32_t programCounter = *los_registros_de_la_cpu -> PC;
        programCounter++;
        *los_registros_de_la_cpu -> PC = programCounter;
    }
}

void solicitar_instruccion(int programCounter){
    t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION);
    agregar_int_a_paquete(paquete, programCounter);
    enviar_paquete(paquete, cpu_cliente_memoria);
    eliminar_paquete(paquete);
    atender_memoria_cpu_sin_while();
    //printf("%s",instruccion_a_decodificar);
}

int decodificar_instruccion(){
    // instruccion_a_decodificar
    char** instruccion_separada = string_split(instruccion_a_decodificar, " ");

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
        //printf("%d\n", *registro_destino2);
    } else if (tamano_destino == 32) {
        int32_t* registro_destino2 = registro_destino;
        *registro_destino2 += contenido_origen;
        //printf("%d\n", *registro_destino2);
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
        //printf("%d\n", *registro_destino2);
    } else if (tamano_destino == 32) {
        int32_t* registro_destino2 = registro_destino;
        *registro_destino2 -= contenido_origen;
        //printf("%d\n", *registro_destino2);
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


int ejecutar_instruccion (int codigo_instruccion) {
    char** instruccion_separada = string_split(instruccion_a_decodificar, " ");
    switch (codigo_instruccion)
    {
    case SET: // SET (Registro, Valor)
        int numero = atoi(instruccion_separada[2]);
        set(instruccion_separada[1], numero);
        return 1;
    case SUM: // SUM (Registro Destino, Registro Origen)
        sum(instruccion_separada[1], instruccion_separada[2]);
        return 1;
    case SUB: // SUB (Registro Destino, Registro Origen)
        sub(instruccion_separada[1], instruccion_separada[2]);
        return 1;
    case JNZ: // JNZ (Registro, Instrucción)
        int nuevo_pc = atoi(instruccion_separada[2]);
        jnz(instruccion_separada[1], nuevo_pc);
        return 1;
    case IO_GEN_SLEEP: // IO_GEN_SLEEP (Interfaz, Unidades de trabajo)
        printf("DORMITE");
        return 1;
    case EXIT:
        return 0;
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