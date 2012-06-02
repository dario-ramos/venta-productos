#pragma once
#define DIRECTORIO "/root/Desktop/trunk/ventaProductos"
#define TICKETS 50
#define VENDEDORES 4
#define COMPRAS 3
#define PRECIO 11
#define MONTO_BASE 9

#define PUERTO 5000

#define SHM_NUM 1
#define SEM_NUM (SHM_NUM + 1)
#define COLA_RECEPCION (SHM_NUM + 2) //vendedor
#define COLA_DESPACHO (SHM_NUM + 3) //vendedor
#define COLA_SALIDA (SHM_NUM + 5)  //cliente
#define COLA_ENTRADA (SHM_NUM + 6)  //cliente

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "inet.h"
#include "mensajes.h"
#include <signal.h>

namespace mensajes{

	typedef struct {
		long int cliente;
		int monto;
	}DatosCompra;

	typedef struct{
		long int tipo;
		DatosCompra compra;
	}MsjCompra;

	typedef struct{
		long int cliente;
		int resultado; //0 = exito, 1 = falta dinero, 2 = se terminaron las entradas
		int vuelto;
		int nro_ticket;
	}DatosRespuesta;

	typedef struct{
		long int tipo;
		DatosRespuesta respuesta;
	}MsjRespuesta;
}
