#include <main_cpu.h>

int main(int argc, char* argv[]) {

    t_log* logger;
	t_config* config;
    
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    int PUERTO_ESCUCHA_DISPATCH;
    int PUERTO_ESCUCHA_INTERRUPT;
    int CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;

    config = iniciar_config("./cpu_config.config");
    logger = iniciar_logger("./cpu.log", "Cpu_Logger", LOG_LEVEL_INFO);

    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    CANTIDAD_ENTRADAS_TLB = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    ALGORITMO_TLB = config_get_string_value(config, "ALGORITMO_TLB");

    //Conexion a Memoria
    int conexion_a_memoria;
    conexion_a_memoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    printf("Funcionando");
    enviar_mensaje("Hola, soy el CPU", conexion_a_memoria);
    liberar_conexion(conexion_a_memoria);

    decir_hola("CPU");
    return 0;
}


