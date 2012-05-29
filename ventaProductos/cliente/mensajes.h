#ifndef MENSAJES_H
#define MENSALES_H
#include <sys/msg.h>

#ifdef _SLAX_
inline int enviar_mensaje(int qid, const void * msg, size_t msgsz){
	return msgsnd(qid, msg, msgsz - sizeof(long), 0);
}

inline int recibir_mensaje(int qid, void * msg, size_t msgsz, long msgtyp){
	return msgrcv(qid, msg, msgsz - sizeof(long), msgtyp, 0);
}
#else
inline int enviar_mensaje(int qid, const void * msg, size_t msgsz){
	return msgsnd(qid, msg, msgsz - sizeof(long), 0);
}

inline int recibir_mensaje(int qid, void * msg, size_t msgsz, long msgtyp){
	return msgrcv(qid, msg, msgsz - sizeof(long), msgtyp, 0);
}
#endif

#endif

