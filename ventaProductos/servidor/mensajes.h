#ifndef MENSAJES_H
#define MENSALES_H
#include <sys/msg.h>

#ifdef _SLAX_
inline int enviar_mensaje(int qid, const void * msg, size_t msgsz){
	char mostrar[200];
	sprintf(mostrar,"mensajes-%d: Va a enviar %lu bytes a la cola\n", getpid(), msgsz);
	write(fileno(stdout), mostrar, strlen(mostrar));
	return msgsnd(qid, msg, msgsz - sizeof(long), 0);
}

inline int recibir_mensaje(int qid, void * msg, size_t msgsz, long msgtyp){
	char mostrar[200];
	sprintf(mostrar,"mensajes-%d: Va a reciir %lu bytes a la cola %i, con msgtype = %li\n", getpid(), msgsz, qid, msgtyp);
	write(fileno(stdout), mostrar, strlen(mostrar));
    return msgrcv(qid, msg, msgsz - sizeof(long), msgtyp, 0);
}
#else
inline int enviar_mensaje(int qid, const void * msg, size_t msgsz){
	char mostrar[200];
	sprintf(mostrar,"mensajes-%d: VIENE POR EL QUE NO ES %lu bytes a la cola\n", getpid(), msgsz);
	write(fileno(stdout), mostrar, strlen(mostrar));
	return msgsnd(qid, msg, msgsz - sizeof(long), 0);
}

inline int recibir_mensaje(int qid, void * msg, size_t msgsz, long msgtyp){
	char mostrar[200];
	sprintf(mostrar,"mensajes-%d: POR EL QUE NO ES Va a reciir %lu bytes a la cola %i, con msgtype = %li\n", getpid(), msgsz, qid, msgtyp);
	write(fileno(stderr), mostrar, strlen(mostrar));
    return msgrcv(qid, msg, msgsz - sizeof(long), msgtyp, 0);
}
#endif

#endif

