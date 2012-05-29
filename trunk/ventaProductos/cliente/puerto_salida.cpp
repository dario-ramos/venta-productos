#include "venta_tickets.h"

extern int enviar(int, void *, size_t);


int main(int argc, char ** argv){
	char *pname;
	char mostrar[200];
	int cola;
	key_t clave;
	int sfd;
	int prt_salida_pid;
	int bytes_enviados;
	mensajes::MsjCompra compra;
	mensajes::DatosCompra datos_compra;

	pname = argv[0];
	sfd = atoi(argv[1]);
	prt_salida_pid = atoi(argv[2]);
	
	clave = ftok(DIRECTORIO, COLA_SALIDA);
	if((cola = msgget(clave, 0660)) == -1){
		perror("cliente: error al crear la cola de compras");
		exit(1);		
	}
	
	while(1){
		//sprintf(mostrar,"%s-%d: Va a recibir de la cola %d bytes \n", pname, getpid(), sizeof(compra));
		//write(fileno(stdout), mostrar, strlen(mostrar));
		if(recibir_mensaje(cola, &compra, sizeof(compra), 0) == -1){
			kill(prt_salida_pid, SIGKILL);
			if(errno == EINVAL || errno == EIDRM){
				sprintf(mostrar, "%s-%d: Termina\n", pname, getpid());
				write(fileno(stderr), mostrar, strlen(mostrar));
				exit(0);
			}else{
				sprintf(mostrar, "%s-%d: Error desconocido ACA.\n", pname, getpid());
				perror(mostrar);
				exit(1);
			}
		}
		datos_compra = compra.compra;
		//sprintf(mostrar,"%s-%d: Va a enviar al socket %d bytes\n", pname, getpid(), sizeof(datos_compra));
		//write(fileno(stdout), mostrar, strlen(mostrar));
		bytes_enviados = enviar(sfd, &datos_compra, sizeof(datos_compra));
		if(bytes_enviados != sizeof(datos_compra)){
			sprintf(mostrar, "%s-%d: Error al enviar los datos de compra.\n", pname, getpid());
			perror(mostrar);
			exit(1);
		}
	}
	return 0;
}
