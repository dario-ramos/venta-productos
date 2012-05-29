#include "venta_tickets.h"

extern int enviar(int, void *, size_t);
extern int tcpopact(char *, int);

int main(int argc, char ** argv){
	char *pname;
	char mostrar[200];
	int cola;
	key_t clave;
	int sfd;
	int bytes_enviados;
	mensajes::MsjRespuesta despacho;
	mensajes::DatosRespuesta datos_desp;

	pname = argv[0];
	sfd = atoi(argv[1]);
	
	clave = ftok(DIRECTORIO, COLA_DESPACHO);
	if((cola = msgget(clave, 0660)) == -1){
		perror("cliente: error al crear la cola de compras");
		exit(1);		
	}
	
	while(1){
		if(recibir_mensaje(cola, &despacho, sizeof(despacho), sfd) == -1){
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
		datos_desp = despacho.respuesta;
		bytes_enviados = enviar(sfd, &datos_desp, sizeof(datos_desp));
		if(bytes_enviados != sizeof(datos_desp)){
			sprintf(mostrar, "%s-%d: Error al enviar los datos de compra.\n", pname, getpid());
			perror(mostrar);
			exit(1);
		}
	}
	return 0;
}
