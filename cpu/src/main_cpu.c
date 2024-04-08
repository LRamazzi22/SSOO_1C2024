#include <main_cpu.h>

int main(int argc, char* argv[]) {

    inicializar_cpu();

    //Conexion a Memoria
    int conexion_a_memoria;
    conexion_a_memoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    printf("Funcionando");
    enviar_mensaje("Hola, soy el CPU", conexion_a_memoria);
    liberar_conexion(conexion_a_memoria);

    decir_hola("CPU");
    return 0;
}


