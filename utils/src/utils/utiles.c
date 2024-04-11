#include <utils/utiles.h>

void decir_hola(char* quien) {
    printf("Hola desde %s!!\n", quien);
}

t_config* iniciar_config(char* ruta)
{
	t_config* nuevo_config = config_create(ruta);
	if(nuevo_config == NULL){
		perror("Error al intentar cargar el config");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}

t_log* iniciar_logger(char* ruta, char* nombre, int nivel)
{
	t_log* nuevo_logger = log_create(ruta, nombre, 1, nivel);

	if(nuevo_logger == NULL){
		printf("Error con el logger");
		exit(1);
	}

	return nuevo_logger;
}

void terminar_programa(t_log* logger_destruir, t_config* config_destruir)
{
	log_destroy(logger_destruir);
	config_destroy(config_destruir);
}

//Funciones de Cliente

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto) 
{
	int err;
	struct addrinfo hints;
	struct addrinfo *server_info;

	if (ip == NULL || puerto == NULL) {
        fprintf(stderr, "IP o puerto no pueden ser nulos.\n");
        exit(EXIT_FAILURE);
    }

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(ip, puerto, &hints, &server_info);
	if (err != 0) {
		perror("Error al ejecutar getaddrinfo()");
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

	if (server_info == NULL) {
        fprintf(stderr, "No se pudo obtener la información del servidor.");
		freeaddrinfo(server_info);
        exit(EXIT_FAILURE);
    }

	// Ahora vamos a crear el socket.
	int socket_cliente  = socket(server_info->ai_family,
                    	server_info->ai_socktype,
                    	server_info->ai_protocol);

	if (socket_cliente == -1) {
        perror("Error al crear el socket");
        freeaddrinfo(server_info);
        exit(EXIT_FAILURE);
    }

	// Ahora que tenemos el socket, vamos a conectarlo

	err = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	if (err == -1)
        perror("Error al conectar");
	else
    	printf("Conexión establecida correctamente.\n");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

//Funciones de Server

int iniciar_servidor(char* puerto_de_escucha, t_log* logger)
{

	int socket_servidor;
	struct addrinfo hints, *servinfo;

	// Cargamos la estructura hints con la familia de direcciones que queremos
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// Obtenemos la lista de direcciones que cumplen con los parametros de hints
	getaddrinfo(NULL, puerto_de_escucha, &hints, &servinfo); 

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
                         servinfo->ai_socktype,
                         servinfo->ai_protocol);
	// Asociamos el socket a un puerto
	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	// Escuchamos las conexiones entrantes

	listen(socket_servidor, SOMAXCONN);
	printf("Estoy escuchando en el puerto %s\n", puerto_de_escucha);

	freeaddrinfo(servinfo);
	log_info(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log* logger, char* mensaje)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	printf("Recibi una conexion en el socket %d!!\n", socket_cliente);
	log_info(logger, mensaje);

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente, t_log* logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje: %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}