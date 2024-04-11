#include <atender_mensajes.h>

void atender_kernel_entradasalida(){
     while (1) {
        printf("Esperando mensajes de Kernel\n");
		int cod_op = recibir_operacion(entradasalida_cliente_kernel);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(entradasalida_cliente_kernel, logger);
			break;
		case -1:
			log_error(logger, "Se desconceto el Kernel");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}

void atender_memoria_entradasalida(){
    while (1) {
        printf("Esperando mensajes de Memoria\n");
		int cod_op = recibir_operacion(entradasalida_cliente_memoria);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(entradasalida_cliente_memoria, logger);
			break;
		case -1:
			log_error(logger, "Se desconceto la Memoria");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}  
}