#include "Vendedor.h"
#include "semaforo.h"

Vendedor::Vendedor(){
	error = !inicializar();
}

int Vendedor::precio_ticket(){
	p(mutex);
	int precio = shm1->precio;
	v(mutex);
	return precio;

}
int Vendedor::get_ticket(){
	int nroTicket;
	p(mutex);
	if(shm1->cantTickets > 0){
		nroTicket = shm1->proxNroTicket++;
		shm1->cantTickets--;
	}else{
		nroTicket = -1;
	}
	v(mutex);
	return nroTicket;
}

bool Vendedor::recibir_peticion(mensajes::MsjCompra * compra, long msgtyp){
	char mostrar[200];
	sprintf(mostrar, "Vendedor-%d: Va a recibir %lu bytes a la cola %d\n", getpid(), sizeof(*compra), recep_qid);
	write(fileno(stderr), mostrar, strlen(mostrar));
	
	int cant = recibir_mensaje(recep_qid, compra, sizeof(mensajes::MsjCompra), msgtyp);
	
	return cant != -1;
}

bool Vendedor::enviar_respuesta(const mensajes::MsjRespuesta * respuesta){
	int err = enviar_mensaje(envio_qid, respuesta, sizeof(*respuesta));
	return err == 0;
}

bool Vendedor::inicializar(){
	key_t clave;
	clave = ftok(DIRECTORIO, SHM_NUM);
	if((shmid = shmget(clave, sizeof(Estado), 0660)) == -1){
		perror("vendedor: error obteniendo la memoria compartida");
		return false;
	}
	if((shm1 = (Estado *) shmat(shmid, 0, 0)) == (Estado * ) -1){
		perror("vendedor: error al vincular la memoria compartida");
		return false;
	}
	
	/*Creo el semaforo */
	if((mutex = getsem(SEM_NUM)) == -1){
		perror("vendedor: error al crear el semaforo");
		return false;
	}

	/*Creo la cola de recepcion*/
	clave = ftok(DIRECTORIO, COLA_RECEPCION);
	if((recep_qid = msgget(clave, 0660)) == -1){
		perror("vendedor: error al crear la cola de recepcion");
		return false;	
	}

	/*Creo la cola de despacho*/
	clave = ftok(DIRECTORIO, COLA_DESPACHO);
	if((envio_qid = msgget(clave, 0660)) == -1){
		perror("vendedor: error al crear la cola de despacho");
		return false;	
	}
	return true;
}

