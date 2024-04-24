#include <crear_pcb.h>

pcb* creacion_pcb(char* ruta_pseudocodigo){
    pcb* el_pcb = malloc(sizeof(pcb));
    el_pcb ->estado_proceso = NEW;
    el_pcb ->PID = crear_pid();
    el_pcb ->quantum_restante = QUANTUM;
    t_paquete* paquete_codeop_ruta = crear_paquete(CREAR_PROCESO);
    agregar_string_a_paquete(paquete_codeop_ruta,ruta_pseudocodigo);
    enviar_paquete(paquete_codeop_ruta,kernel_cliente_memoria);
    eliminar_paquete(paquete_codeop_ruta);
    int programCounter = recibir_PC_memoria();
    if(programCounter <0){
        log_error(logger, "Error al recibir el program counter");
        exit(EXIT_FAILURE);
    }
    el_pcb ->registros_cpu_en_pcb.PC = programCounter;
    el_pcb ->registros_cpu_en_pcb.AX = 0;
    el_pcb ->registros_cpu_en_pcb.BX = 0;
    el_pcb ->registros_cpu_en_pcb.CX = 0;
    el_pcb ->registros_cpu_en_pcb.DX = 0;
    el_pcb ->registros_cpu_en_pcb.EAX = 0;
    el_pcb ->registros_cpu_en_pcb.EBX = 0;
    el_pcb ->registros_cpu_en_pcb.ECX = 0;
    el_pcb ->registros_cpu_en_pcb.EDX = 0;
    el_pcb ->registros_cpu_en_pcb.DI = 0;
    el_pcb ->registros_cpu_en_pcb.SI = 0;

    return el_pcb;

}

int crear_pid(){
    int pid;
    pthread_mutex_lock(&mutex_para_pid);
    pid = pid_acumulado;
    pid_acumulado++;
    pthread_mutex_unlock(&mutex_para_pid);
    return pid;
}