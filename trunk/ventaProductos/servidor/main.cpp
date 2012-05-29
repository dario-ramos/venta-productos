#include "venta_tickets.h"

#include "Vendedor.h"

using namespace mensajes;
int main(int argc, char ** argv){
	MsjCompra peticion;
	DatosCompra datos_compra;
	MsjRespuesta respuesta;
	DatosRespuesta datos_respuesta;
	char mostrar[200];
	char * pname;
	char * pnum;
	bool continua = true;
	Vendedor vendedor;
	int precio;

	pname = argv[0];
	pnum = argv[1];
	
	srand(time(NULL));
	if(vendedor){
		sprintf(mostrar,"%s%s-%d: IPCs necesarios creados correctamente.  Ejecutando loop principal.\n", pname, pnum, getpid());
		write(fileno(stdout), mostrar, strlen(mostrar));
	}else{
		sprintf(mostrar,"%s%s-%d: Error al crear el vendedor.\n", pname, pnum, getpid());
		write(fileno(stdout), mostrar, strlen(mostrar));
		return 1;
	}
	
	while(continua){
		int tiempo = rand() % 10;
		sleep(tiempo);
		sprintf(mostrar, "%s%s-%d: Va a recibir %lu bytes a la cola \n", pname, pnum, getpid(), sizeof(peticion));
		write(fileno(stderr), mostrar, strlen(mostrar));
		if(!vendedor.recibir_peticion(&peticion, 0)){
			if(errno == EINVAL || errno == EIDRM){
				sprintf(mostrar, "%s-%d: Termina\n", pname, getpid());
				write(fileno(stderr), mostrar, strlen(mostrar));
				return 0;
			}else{
				sprintf(mostrar, "%s%s-%d: Error desconocido ACA.\n", pname, pnum, getpid());
				perror(mostrar);
				return 1;
			}
		}

		datos_compra = peticion.compra;
		precio = vendedor.precio_ticket();
		if( precio > datos_compra.monto){
			datos_respuesta.resultado = 1;/*Falta dinero*/
			datos_respuesta.vuelto = peticion.compra.monto;
			datos_respuesta.nro_ticket = -1;				
		}else {
			datos_respuesta.nro_ticket = vendedor.get_ticket();
			if(datos_respuesta.nro_ticket < 0){
				datos_respuesta.resultado = 2;
				datos_respuesta.vuelto = datos_compra.monto;
			}else{
				datos_respuesta.resultado = 0;
				datos_respuesta.vuelto = datos_compra.monto - precio;
			}
		}
		datos_respuesta.cliente = datos_compra.cliente;
		respuesta.tipo = peticion.tipo;
		respuesta.respuesta = datos_respuesta;
		if(datos_respuesta.resultado == 1){
			sprintf(mostrar, "%s%s-%d: Monto Insufuciente.\n", pname, pnum, getpid());
		}else if (datos_respuesta.resultado == 2)
			sprintf(mostrar, "%s%s-%d: Se terminaron los tickets.\n", pname, pnum, getpid());
		else
			sprintf(mostrar, "%s%s-%d: Compra exitosa, NroTicket: %i; Vuelto: %i\n", pname, pnum, getpid(), datos_respuesta.nro_ticket, datos_respuesta.vuelto);
		write(fileno(stdout), mostrar, strlen(mostrar));
		sprintf(mostrar, "%s%s-%d: Va a enviar %lu bytes a la cola\n", pname, pnum, getpid(), sizeof(peticion));
		write(fileno(stderr), mostrar, strlen(mostrar));
		if(!vendedor.enviar_respuesta( &respuesta)){
			if(errno == EINVAL || errno == EIDRM){
				sprintf(mostrar, "%s-%d: Termina\n", pname, getpid());
				write(fileno(stderr), mostrar, strlen(mostrar));
				return 0;
			}else{
				sprintf(mostrar, "%s-%d: Error desconocido.\n", pname, getpid());
				perror(mostrar);
				return 1;
			}
		}

	}
	return 0;
}
