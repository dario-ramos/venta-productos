#include "venta_tickets.h"
#include "semaforo.h"

extern int tcpoppas(int puerto);

int main(int argc, char ** argv){
	Estado *shm1;
	int shmid, mutex;
	int recepcion, despacho;
	key_t clave;
	struct sockaddr_in cli_addr;
	char mostrar[200];
	int tickets;
	int vendedores;
	int puerto;
	int sockfd, newsockfd;
	static char par1[8];
	char *pname;
	int childpid;
	int ret = 0;
	unsigned clilen;
	bool continua = true;
	int i = 0;
	
	if(argc > 3){
		tickets = atoi(argv[1]);
		vendedores = atoi(argv[2]);
 		puerto = atoi(argv[3]);
	}else if(argc > 2){
		tickets = atoi(argv[1]);
		vendedores = atoi(argv[2]);
		puerto = PUERTO;
	}else if(argc > 1){
		tickets = atoi(argv[1]);
		vendedores = VENDEDORES;
		puerto = PUERTO;
	}else{
		tickets = TICKETS;
		vendedores = VENDEDORES;		
		puerto = PUERTO;
	}
	pname = argv[0];

	/*Creo la memoria compartida */
	clave = ftok(DIRECTORIO, SHM_NUM);
	if((shmid = shmget(clave, sizeof(Estado), IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("servidor: error obteniendo la memoria compartida");
		exit(1);
	}
	if((shm1 = (Estado *) shmat(shmid, 0, 0)) == (Estado * ) -1){
		perror("servidor: error al vincular la memoria compartida");
		exit(1);
	}
	shm1->cantTickets = tickets;
	shm1->precio = PRECIO;
	shm1->proxNroTicket = 1;
	shm1->caja = 0;
	
	/*Creo el semaforo */
	if((mutex = creasem(SEM_NUM)) == -1){
		perror("servidor: error al crear el semaforo");
		exit(1);
	}
	if(inisem(mutex, 1)){
		perror("servidor: error al inicializar el semaforo");
		exit(1);
	}
	sprintf(mostrar, "servidor: MutexId = %d.\n", mutex);
	write(fileno(stdout), mostrar, strlen(mostrar));
	
	
	/*Creo la cola de recepcion*/
	clave = ftok(DIRECTORIO, COLA_RECEPCION);
	if((recepcion = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("servidor: error al crear la cola de recepciones");
		exit(1);		
	}

	/*Creo la cola de despacho*/
	clave = ftok(DIRECTORIO, COLA_DESPACHO);
	if((despacho = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("servidor: error al crear la cola de despacho");
		exit(1);		
	}

	if ( (sockfd = tcpoppas(puerto)) < 0){
		perror("servidor: no se puede abrir el stream socket");
		exit(1);
	}
	sprintf (mostrar,"servidor: se hizo el open pasivo, socket %d\n", sockfd);
	write(fileno(stdout), mostrar, strlen(mostrar));
	for(i = 0; i < vendedores; i++){
		sprintf(par1,"%d", i +1);
		//sprintf(mostrar, "%s-%d:Crea vendedor nro: %d \n", pname, 	getpid(), i);
		//write(stderr->_fileno, mostrar, strlen(mostrar));
		if((childpid = fork()) < 0){
			perror("servidor: error al crear vendedor");
			continua = false;
        		ret = 1; 
			continue;
		}else if(childpid == 0){
			execl("./vendedor", "vendedor", par1, (char *) 0);
			perror("servidor: error al ejecutar un vendedor");
			continua = false;
        		ret = 1;
			continue;
		}
	}

	while(continua){
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			perror ("servidor: error en el accept");
			continua = false;
        		ret = 1; 
			continue;
        	}
		//sprintf(mostrar, "%s-%d:Crea puerto_entrada con socket nro: %d \n", pname, 	getpid(), newsockfd);
		//write(stderr->_fileno, mostrar, strlen(mostrar));
		sprintf(par1, "%d", newsockfd);
		if((childpid = fork()) < 0){
			perror("servidor: error al crear puerto de entrada");
			continua = false;
        		ret = 1; 
			continue;
		}else if(childpid == 0){
			execl("./puerto_entrada", "puerto_entrada", par1, (char *) 0);
			perror("servidor: error al ejecutar un cliente");
			continua = false;
        		ret = 1;
			continue;
		}
		//sprintf(mostrar, "%s-%d:Crea puerto_salida con socket nro: %d \n", pname, 	getpid(), newsockfd);
		//write(stderr->_fileno, mostrar, strlen(mostrar));
		if((childpid = fork()) < 0){
			perror("servidor: error al crear puerto de salida");
			continua = false;
        		ret = 1; 
			continue;
		}else if(childpid == 0){
			execl("./puerto_salida", "puerto_salida", par1, (char *) 0);
			perror("servidor: error al ejecutar un cliente");
			continua = false;
        		ret = 1;
			continue;
		}
		sleep(1);
	}
	close(sockfd);
	return ret;
}
