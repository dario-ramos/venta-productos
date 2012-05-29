#ifndef VENDEDOR_H
#define VENDEDOR_H
#include "venta_tickets.h"


class Vendedor{
public:
	Vendedor();
	~Vendedor(){}
	
	int precio_ticket();
	int get_ticket();
	bool recibir_peticion(mensajes::MsjCompra * compra, long msg_type);
	bool enviar_respuesta(const mensajes::MsjRespuesta * respuesta);
	operator bool() const {return !error;}

private:
	Estado * shm1;
	int shmid;
	int mutex;
	int recep_qid;
	int envio_qid;
	bool inicializar();
	bool error;
};
#endif
