#include "server.h"
void* atender_cliente(void* arg);

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);
	

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	

	
	while (1) {
		int arg = esperar_cliente(server_fd);
		int* arg_ptr = malloc(sizeof(int));
    	*arg_ptr = arg;


		pthread_t cliente;
		pthread_create(&cliente, NULL, atender_cliente, (void*) arg_ptr);
		pthread_detach(cliente);
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void* atender_cliente(void* arg) {
	int cliente_fd = *(int*)arg;
	free(arg);
	
	t_list* lista;
	bool titulo = true;
	
	while(1){
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);

			if(titulo){
			log_info(logger, "Me llegaron los siguientes valores:\n");
			titulo = false;
			}

			list_iterate(lista, (void*) iterator);
			list_destroy_and_destroy_elements(lista, free);

			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}
