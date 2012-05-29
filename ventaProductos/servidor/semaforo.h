#ifndef SEMAFORO_H
#define SEMAFORO_H

#ifndef DIRECTORIO
#define DIRECTORIO "/root/Desktop/ventaTickets"
#endif
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>



int creasem(int num){
	key_t clave;
	clave = ftok(DIRECTORIO, num);
	return (semget(clave, 1, IPC_CREAT | IPC_EXCL |0660));
}

int getsem(int n){
	key_t clave;
	clave = ftok(DIRECTORIO, n);
	return (semget(clave, 1, 0660));
}

int inisem(int semid, int val){
	union semnum {
		int val;
		struct semid_ds * buf;
		ushort * array;
		struct seminfo * _buf;
	} arg;

	arg.val = val;
	return (semctl(semid, 0, SETVAL, arg));
}

int p(int semid){
	struct sembuf oper;
	oper.sem_num = 0;
	oper.sem_op = -1;
	oper.sem_flg = 0;
	return (semop(semid, &oper, 1));
}

int v(int semid){
	struct sembuf oper;
	oper.sem_num = 0;
	oper.sem_op = 1;
	oper.sem_flg = 0;
	return (semop(semid, &oper, 1));
}

int elisem(int semid){
	return (semctl (semid, 0, IPC_RMID, (struct semid_ds *) 0));
}

#endif
