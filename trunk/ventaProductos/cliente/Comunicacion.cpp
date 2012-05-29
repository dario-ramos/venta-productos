#include "Comunicacion.h"

extern int tcpopact(char *, int);

Comunicacion::Comunicacion(char * nombr_serv, int puert) : nombre_serv(nombr_serv), puerto(puert){
	_error = inicializarComunicacion() != 0;
} 

Comunicacion::~Comunicacion(){
	finalizarComunicacion();
}

#ifdef _SLAX_
int Comunicacion::enviar_mensaje(const void *msg, int msg_size){
	return msgsnd(envio_qid, msg, msg_size - sizeof(long), 0);
}
int Comunicacion::recibir_mensaje(void *msg, int msg_size, long msgtyp){
	return msgrcv(recep_qid, msg, msg_size - sizeof(long), msgtyp, 0);
}
#else
int Comunicacion::enviar_mensaje(const void *msg, int msg_size){
	return msgsnd(envio_qid, msg, msg_size, 0);
}
int Comunicacion::recibir_mensaje(void *msg, int msg_size, long msgtyp){
	return msgrcv(recep_qid, msg, msg_size, msgtyp, 0);
}
#endif

int Comunicacion::inicializarComunicacion(){
	key_t clave;
	char mostrar[200];
	static char par1[8], par2[8];
	int childpid;
	int sfd;

	/*Creo la cola de peticiones*/
	clave = ftok(DIRECTORIO, COLA_SALIDA);
	sprintf(mostrar, "VA A CREAR LA COLA DE ENVIO con DIRECTORIO = %s y numero =%d\n", DIRECTORIO, COLA_SALIDA);
	write(fileno(stdout), mostrar, strlen(mostrar));
	if((envio_qid = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de peticiones");
		return 1;	
	}

	/*Creo la cola de respuesta*/
	clave = ftok(DIRECTORIO, COLA_ENTRADA);
	sprintf(mostrar, "VA A CREAR LA COLA DE ENVIO con DIRECTORIO = %s y numero =%d", DIRECTORIO, COLA_ENTRADA);
	write(fileno(stdout), mostrar, strlen(mostrar));
	if((recep_qid = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;		
	}
	/*Hago open activo*/
	//sprintf(mostrar, "inicializarComunicacion: Se va a hacer el open activo al servidor %s en el puerto %d.\n", nombre_servidor, puerto);
	//write(fileno(stdout), mostrar, strlen(mostrar));
	sfd = tcpopact(nombre_serv, puerto);

	sprintf(par1, "%d", sfd);
	if((childpid = fork()) < 0){
		perror("inicializarComunicacion: error al crear puerto de entrada");
		return 1;
	}else if(childpid == 0){
		execl("./puerto_entrada", "puerto_entrada", par1, (char *) 0);
		perror("inicializarComunicacion: error al ejecutar el puerto de entrada");
		return 1;
	}
	sprintf(par2, "%d" ,childpid);
	if((childpid = fork()) < 0){
		perror("lanza_cliente: error al crear cliente");
		return 1;
	}else if(childpid == 0){
		execl("./puerto_salida", "puerto_salida", par1, par2, (char *) 0);
		perror("inicializarComunicacion: error al ejecutar un cliente");
		return 1;
	}
	return 0;	
}

void Comunicacion::finalizarComunicacion(){

 	if(msgctl(envio_qid, IPC_RMID, (msqid_ds *) 0)){
		perror("Comunicacion: error al borrar la cola de envio.");
	}

	if(msgctl(recep_qid, IPC_RMID, (msqid_ds *) 0)){
		perror("Comunicacion: error al borrar la cola de recepcion");
	}
}



