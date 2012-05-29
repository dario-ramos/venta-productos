#include "venta_tickets.h"
#include "Cliente.h"

int main(int argc, char ** argv){
	DatosCompra compra;
	DatosRespuesta respuesta;
	char mostrar[200];
	char * pname;
	char * nombre_servidor;
	int puerto;
	int compras;
	int numCliente;
	Cliente * cliente;

	if(argc < 2){
		sprintf(mostrar, "cliente: error, debe ingresar el nombre del servidor.\n");
		write(fileno(stdout), mostrar, strlen(mostrar));
		exit(1);
	}
	pname = argv[0];
	nombre_servidor = argv[1];
	if (argc > 3){
		puerto = atoi(argv[2]);
		compras = atoi(argv[3]);
	}else if(argc > 2){
		puerto = atoi(argv[2]);
		compras = COMPRAS;
	}else {
		puerto = PUERTO;
		compras = COMPRAS;
	}
	srand(time(NULL));
	numCliente = rand() % 1000;

	sprintf(mostrar, "%s%i-%d: Ejecutando.\n", pname, numCliente, getpid());
	write(fileno(stdout), mostrar, strlen(mostrar));

	cliente = new Cliente(nombre_servidor, puerto);

	if(!(*cliente)){
		sprintf(mostrar,"%s%i-%d: Error al crear la capa de transporte. \n", pname, numCliente, getpid());
		perror(mostrar);	
		return 1;
	}
	sprintf(mostrar,"%s%i-%d: Cliente creado correctamente.  Ejecutando compra.\n", pname, numCliente, getpid());
	write(fileno(stdout), mostrar, strlen(mostrar));
	for(int i = 0; i < compras; i++){
		compra.cliente = numCliente;
		compra.monto = MONTO_BASE + i;
		sprintf(mostrar, "%s%i-%d: COMPRANDO %d vez\n", pname, numCliente, getpid(), i + 1);
		write(fileno(stderr), mostrar, strlen(mostrar));
		
		if( !cliente->comprar(&compra, &respuesta) ){
			perror("Error al ejecutar la compra.");
			break;			
		}		
	
		if(respuesta.resultado == 1){
			sprintf(mostrar, "%s%i-%d: Monto Insufuciente.\n", pname, numCliente, getpid());
		}else if (respuesta.resultado == 2)
			sprintf(mostrar, "%s%i-%d: Se terminaron los tickets.\n", pname, numCliente, getpid());
		else
			sprintf(mostrar, "%s%i-%d: Compra exitosa, NroTicket: %i; Vuelto: %i\n", pname, numCliente, getpid(), respuesta.nro_ticket, respuesta.vuelto);
		write(fileno(stdout), mostrar, strlen(mostrar));
	}
	delete cliente;
	return 0;	

}
