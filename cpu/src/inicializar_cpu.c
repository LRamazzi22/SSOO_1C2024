#include <inicializar_cpu.h>

void inicializar_cpu(){
    logger = iniciar_logger("./cpu.log", "Cpu_Logger", LOG_LEVEL_INFO, 1);
    logger_obligatorio = iniciar_logger("./cpuObligatorio.log", "Cpu_Logger_Obligatorio", LOG_LEVEL_INFO,1);
    pid_en_ejecucion = 0;
    interrupcion_recibida = NO_INTERRUPCION;
    pid_de_interrupcion = 0;
    inicializar_config_cpu();

    los_registros_de_la_cpu = iniciar_registros_cpu();

    tlb = list_create();
}

void inicializar_config_cpu(){
    config = iniciar_config("./cpu_config.config");

    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    CANTIDAD_ENTRADAS_TLB = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    ALGORITMO_TLB = config_get_string_value(config, "ALGORITMO_TLB");
}

t_registros_cpu* iniciar_registros_cpu(){
    t_registros_cpu* registro = malloc(sizeof(t_registros_cpu));

    registro->PC = calloc(1,sizeof(uint32_t));
    registro->AX = calloc(1,sizeof(uint8_t));
    registro->BX = calloc(1,sizeof(uint8_t));
    registro->CX = calloc(1,sizeof(uint8_t));
    registro->DX = calloc(1,sizeof(uint8_t));
    registro->EAX = calloc(1,sizeof(uint32_t));
    registro->EBX = calloc(1,sizeof(uint32_t));
    registro->ECX = calloc(1,sizeof(uint32_t));
    registro->EDX = calloc(1,sizeof(uint32_t));
    registro->SI = calloc(1,sizeof(uint32_t));
    registro->DI = calloc(1,sizeof(uint32_t));

    return registro;

}
