#include <main_kernel.h>

int main(int argc, char* argv[]) {

    t_log* logger;
	t_config* config;

    int PUERTO_ESCUCHA;
    char* IP_MEMORIA;
    int PUERTO_MEMORIA;
    char* IP_CPU;
    int PUERTO_CPU_DISPATCH;
    int PUERTO_CPU_INTERRUPT;
    char* ALGORITMO_PLANIFICACION;
    int QUANTUM;
    char* RECURSOS;
    char* INSTANCIAS_RECURSOS;
    int GRADO_MULTIPROGRAMACION;

    config = iniciar_config("./kernel_config.config");
    logger = iniciar_logger("./kernel.log", "Kernel_Logger", LOG_LEVEL_INFO);

    PUERTO_ESCUCHA = config_get_int_value(config, "PUERTO_ESCUCHA");
    IP_MEMORIA = config_get_array_value(config,"IP_MEMORIA");
    PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");
    IP_CPU = config_get_array_value(config,"IP_CPU");
    PUERTO_CPU_DISPATCH = config_get_int_value(config, "PUERTO_CPU_DISPATCH");
    PUERTO_CPU_INTERRUPT = config_get_int_value(config, "PUERTO_CPU_INTERRUPT");
    ALGORITMO_PLANIFICACION = config_get_array_value(config,"ALGORITMO_PLANIFICACION");
    QUANTUM = config_get_int_value(config, "QUANTUM");
    RECURSOS = config_get_array_value(config,"RECURSOS");
    INSTANCIAS_RECURSOS = config_get_array_value(config,"INSTANCIAS_RECURSOS");
    GRADO_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    

    decir_hola("Kernel");
    
    return 0;
}
