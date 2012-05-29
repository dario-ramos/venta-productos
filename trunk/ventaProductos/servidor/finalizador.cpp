#include "venta_tickets.h"
#include "semaforo.h"

int main(int argc, char ** argv){
	int shmid, mutex;
	int recepcion, despacho;
	key_t clave;
	char *pname;



	pname = argv[0];

	/*Creo la memoria compartida */
	clave = ftok(DIRECTORIO, SHM_NUM);
	if((shmid = shmget(clave, sizeof(Estado),  0660)) == -1){
		perror("servidor: error obteniendo la memoria compartida");
		exit(1);
	}else if(shmctl(shmid, IPC_RMID, (shmid_ds *) 0)){
		perror("finalizador: error al borrar la cola de compras");
	}
	
	/*Creo el semaforo */
	if((mutex = getsem(SEM_NUM)) == -1){
		perror("servidor: error al crear el semaforo");
		exit(1);
	}else if(elisem(mutex)){
		perror("finalizador: error al borrar el semaforo");
	}
	
	
	/*Creo la cola de recepcion*/
	clave = ftok(DIRECTORIO, COLA_RECEPCION);
	if((recepcion = msgget(clave, 0660)) == -1){
		perror("servidor: error al crear la cola de compras");
		exit(1);		
	}else if(msgctl(recepcion, IPC_RMID, (msqid_ds *) 0)){
		perror("finalizador: error al borrar la cola de compras");
	}

	/*Creo la cola de despacho*/
	clave = ftok(DIRECTORIO, COLA_DESPACHO);
	if((despacho = msgget(clave, 0660)) == -1){
		perror("servidor: error al crear la cola de despacho");
		exit(1);		
	}else if(msgctl(despacho, IPC_RMID, (msqid_ds *) 0)){
		perror("finalizador: error al borrar la cola de despacho");
	}

	
	return 0;	
}
