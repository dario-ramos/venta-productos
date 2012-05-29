#ifndef Cliente_H
#define Cliente_H
#include "Comunicacion.h"
#include "venta_tickets.h"
using namespace mensajes;
class Cliente{
public:
	Cliente(char * ip_servidor, int puerto) : comunicacion(ip_servidor, puerto), error(false){}
	~Cliente(){}
    bool obtenerId();
	bool comprar(const DatosCompra *datos, DatosRespuesta *respuesta);
	operator bool() const {return comunicacion;}

private:
	Comunicacion comunicacion;
	bool error;
};

bool Cliente::comprar(const DatosCompra *datos, DatosRespuesta *respuesta){
	MsjCompra msj_compra;
	MsjRespuesta msj_respuesta;
	char mostrar[200];

	msj_compra.compra = *datos;
	msj_compra.tipo = 1;
	int numCliente = datos->cliente;

	if(comunicacion.enviar_mensaje(&msj_compra, sizeof(msj_compra)) == -1){
		if(errno == EINVAL || errno == EIDRM){
			sprintf(mostrar, "Cliente%i-%d: Termina\n", numCliente, getpid());
			write(fileno(stderr), mostrar, strlen(mostrar));
		}else{
			sprintf(mostrar, "Cliente%i-%d: Error desconocido.\n", numCliente, getpid());
			perror(mostrar);
			error = true;
		}
		return false;
	}

	if(comunicacion.recibir_mensaje(&msj_respuesta, sizeof(msj_respuesta), numCliente) == -1){
		if(errno == EINVAL || errno == EIDRM){
			sprintf(mostrar, "Cliente%i-%d: Termina\n", numCliente, getpid());
			write(fileno(stderr), mostrar, strlen(mostrar));
		}else{
			sprintf(mostrar, "Cliente%i-%d: Error desconocido ACA.\n", numCliente, getpid());
			perror(mostrar);
			error = true;

		}
		return false;
	}
	*respuesta = msj_respuesta.respuesta;
	return true;
}

#endif

