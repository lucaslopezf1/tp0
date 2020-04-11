/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	log_info(logger,"-----------LOG START--------");
	//Loggear "soy un log"

	config = leer_config();


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	log_info(logger,puerto);
	log_info(logger,ip);

	int socket = crear_conexion(ip,puerto);
	enviar_mensaje("hola",socket);

	char* mensaje = recibir_mensaje(socket);

	log_info(logger,mensaje);
	
	free(mensaje);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* logger = log_create("tp0.log","Prueba",true,LOG_LEVEL_INFO);
	return logger;
}

t_config* leer_config(void)
{
	t_config* config = config_create("tp0.config");
	return config;
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	config_destroy(config);
	log_info(logger,"-----------LOG END--------");
	liberar_conexion(conexion);
	log_destroy(logger);
}
