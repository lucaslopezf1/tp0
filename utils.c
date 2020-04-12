/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{

	void* a_enviar = malloc (2*sizeof(int) + sizeof(paquete->buffer->stream));

	memcpy(a_enviar + *bytes, &paquete->codigo_operacion, sizeof(int));
	*bytes += sizeof(int);
	memcpy(a_enviar  + *bytes, &(paquete -> buffer -> size),sizeof(int));
	*bytes += sizeof(int);
	memcpy(a_enviar  + *bytes, paquete -> buffer -> stream,paquete -> buffer -> size);
	*bytes += sizeof(paquete->buffer->size);

	return a_enviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete -> buffer = malloc(sizeof(t_buffer));
	paquete-> buffer -> stream = malloc(paquete->buffer -> size);

	paquete -> buffer -> size = strlen(mensaje);
	memcpy(paquete-> buffer -> stream, mensaje, paquete-> buffer -> size);
	paquete -> codigo_operacion = MENSAJE;

	int bytes = 0;
	
	void* a_enviar = serializar_paquete( paquete, &(bytes));

	send(socket_cliente,a_enviar,bytes,0);

	free(a_enviar);
	free(paquete -> buffer -> stream);
	free(paquete -> buffer);
	free(paquete);


}

char* recibir_mensaje(int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	char* mensaje;

	recv(socket_cliente,&(paquete -> codigo_operacion),sizeof(paquete -> codigo_operacion),0);
	recv(socket_cliente,&(paquete -> buffer -> size),sizeof(paquete -> buffer -> size),0);
	paquete->buffer->stream = malloc((paquete->buffer->size));
	recv(socket_cliente,&(paquete -> buffer -> stream),paquete -> buffer -> size,0);

	if(paquete -> codigo_operacion == MENSAJE)
		mensaje = deserializar_mensaje(paquete -> buffer);


	free(paquete -> buffer);
	free(paquete);

	return mensaje;
}

char* deserializar_mensaje(t_buffer* buffer){
	char* mensaje = malloc((buffer -> size));

	memcpy(mensaje,&(buffer -> stream) ,buffer -> size);

	return mensaje;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
