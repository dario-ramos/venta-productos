#ifndef VENDEDOR_H
#define VENDEDOR_H
#include "Comunicacion.h"
#include "venta_tickets.h"
using namespace mensajes;
class Vendedor{
	Vendedor(char * ip_vendedor, int puerto) : transporte(ip_vendedor, puerto){}
	~Vendedor(){}
	bool comprar(const datos_compra *datos, datos_respuesta *respuesta);

private:
	Transporte transporte;
};

#endif

