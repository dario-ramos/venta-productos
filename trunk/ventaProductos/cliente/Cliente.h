#ifndef Cliente_H
#define Cliente_H
#include "Comunicacion.h"
#include "venta_tickets.h"
#include "servidor_ids_clientes.h"
using namespace mensajes;
class Cliente{
public:
	Cliente(char * ip_servidor, int puerto);
	~Cliente();
	bool comprar( DatosCompra *datos, DatosRespuesta *respuesta);
	operator bool() const {return comunicacion;}

private:
	Comunicacion comunicacion;
	bool error;
	bool obtenerId();
	bool devolverId();
	int id;
	char * ip_servidor_;
	CLIENT *clnt;
};
Cliente::Cliente(char * ip_servidor, int puerto) : 
				comunicacion(ip_servidor, puerto), 
				error(false),
				ip_servidor_(ip_servidor)
{
	error = !obtenerId();
}

Cliente::~Cliente()
{
	char mostrar[200];
	sprintf(mostrar, "Destructor de cliente\n");
	write(fileno(stderr), mostrar, strlen(mostrar));
	devolverId();
}

bool Cliente::comprar( DatosCompra *datos, DatosRespuesta *respuesta){
	MsjCompra msj_compra;
	MsjRespuesta msj_respuesta;
	char mostrar[200];

	datos->cliente = id;
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

bool Cliente::obtenerId(){
	retorno  *result_1;
	char *obtener_nuevo_id_cliente_1_arg;

	clnt = clnt_create (ip_servidor_, SERVIDOR_IDS_PROG, SERVIDOR_IDS_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (ip_servidor_);
		return false;
	}


	result_1 = obtener_nuevo_id_cliente_1((void*)&obtener_nuevo_id_cliente_1_arg, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "Error al obtener el Id");
		return false;
	}
	id = result_1->retorno_u.id_cliente;

}

bool Cliente::devolverId(){
	retorno  *result_1;
char mostrar[200];
	sprintf(mostrar, "Va a hacer el llamado rpc\n");
	write(fileno(stderr), mostrar, strlen(mostrar));
	result_1 = devolver_id_cliente_1(&id, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "Error al obtener el Id");
		return false;
	}
	sprintf(mostrar, "Hace el llamado\n");
	write(fileno(stderr), mostrar, strlen(mostrar));
	clnt_destroy (clnt);
	return true;

}

#endif

