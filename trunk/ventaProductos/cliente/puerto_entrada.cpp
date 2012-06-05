#include "venta_tickets.h"

extern int recibir(int, void *, size_t);

int main(int argc, char ** argv){
	char *pname;
	char mostrar[200];
	int cola;
	key_t clave;
	int sfd;
	int bytes_recibidos;
	mensajes::MsjRespuesta respuesta;
	mensajes::DatosRespuesta datos_respuesta;

	pname = argv[0];
	sfd = atoi(argv[1]);
	
	clave = ftok(DIRECTORIO, COLA_ENTRADA);
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		printf( "puerto_entrada: El directorio del ftok, %s, no existe\n", DIRECTORIO );
		exit(1);
	}
	if((cola = msgget(clave, 0660)) == -1){
		perror("cliente: error al crear la cola de compras");
		exit(1);		
	}	
	
	while(1){
		sprintf(mostrar,"%s-%d: Va a recibir %d bytes del socket \n", pname, getpid(), sizeof(datos_respuesta));
		write(fileno(stdout), mostrar, strlen(mostrar));

		bytes_recibidos = recibir(sfd, &datos_respuesta, sizeof(datos_respuesta));
		if(bytes_recibidos != sizeof(datos_respuesta)){
			sprintf(mostrar, "%s-%d: Error al enviar los datos de compra.\n", pname, getpid());
			perror(mostrar);
			close(sfd);
			exit(1);
		}
		respuesta.respuesta = datos_respuesta;
		respuesta.tipo = datos_respuesta.cliente;
		sprintf(mostrar,"%s-%d: Va a enviar a la cola  %d  bytes\n", pname, getpid(), sizeof(respuesta));
		write(fileno(stdout), mostrar, strlen(mostrar));
		if(enviar_mensaje(cola, &respuesta, sizeof(respuesta)) == -1){
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
