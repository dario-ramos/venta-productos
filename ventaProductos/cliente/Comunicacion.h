#ifndef COMUNICACION_H
#define COMUNICACION_H
#include <sys/msg.h>
#include <cstring>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "venta_tickets.h"

class Comunicacion{
public:
	Comunicacion(char * nombre_serv, int puerto);
	~Comunicacion();
	int enviar_mensaje(const void * msg, int msg_size);
	int recibir_mensaje(void *, int msg_size, long msgtype);
	operator bool() const { return !_error; }

private:
	int envio_qid;
	int recep_qid;
	char * nombre_serv;
	int puerto;
	bool _error;

	int inicializarComunicacion();
	void finalizarComunicacion();
};




#endif
