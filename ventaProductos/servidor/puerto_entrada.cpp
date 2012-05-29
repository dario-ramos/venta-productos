#include "venta_tickets.h"

extern int recibir(int, void *, size_t);
extern int tcpopact(char *, int);

int main(int argc, char ** argv){
	int sfd;
	char *pname;
	char mostrar[200];
	int cola;
	key_t clave;
	int bytes_recibidos;
	mensajes::MsjCompra peticion;
	mensajes::DatosCompra datos_peticion;

	pname = argv[0];
	sfd = atoi(argv[1]);
	
	clave = ftok(DIRECTORIO, COLA_RECEPCION);
	if((cola = msgget(clave, 0660)) == -1){
		perror("cliente: error al crear la cola de compras");
		exit(1);		
	}
	
	while(1){
		sprintf(mostrar,"%s-%d: Va a recibir %lu bytes del socket\n", pname, getpid(), sizeof(datos_peticion));
		write(fileno(stdout), mostrar, strlen(mostrar));
		bytes_recibidos = recibir(sfd, &datos_peticion, sizeof(datos_peticion));
		if(bytes_recibidos != sizeof(datos_peticion)){
			sprintf(mostrar, "%s-%d: Error al recibir datos de la peticion.\n", pname, getpid());
			perror(mostrar);
			close(sfd);
			exit(1);
		}
		peticion.compra = datos_peticion;
		peticion.tipo = sfd;
		sprintf(mostrar,"%s-%d: Va a enviar %lu bytes a la cola %d\n", pname, getpid(), sizeof(peticion), cola);
		write(fileno(stdout), mostrar, strlen(mostrar));
		if(enviar_mensaje(cola, &peticion, sizeof(peticion)) == -1){
			close(sfd);
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
	}
	return 0;
}
