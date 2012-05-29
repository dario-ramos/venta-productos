/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "servidor_ids_clientes.h"
#define DIRECTORIO "/dario/home/workspace"
#include <sys/sem.h>
#include "semaforos.h"
#define ID_MUTEX_SERVIDOR_RPC 69

retorno * obtener_nuevo_id_cliente_1_svc(void *argp, struct svc_req *rqstp){
	static retorno result;
	static int idMutex = 0;
	static int idCliente = 0;
	result.cod_ret = 2; //Error
	//Crear mutex la primera vez
	if( idMutex == 0 ){
		idMutex = creasem( ID_MUTEX_SERVIDOR_RPC );
		if( idMutex == -1 ){
			strcpy( result.retorno_u.msj_error, "Error al crear mutex" );
			return &result;
		}
		if( inisem( idMutex, 1 ) == -1){
			strcpy( result.retorno_u.msj_error, "Error al inicializar mutex" );
			return &result;
		}
	}
	//Adquirir mutex, obtener id, incrementar y liberar mutex
	if( p(idMutex) == -1 ){
		strcpy( result.retorno_u.msj_error, "Error al hacer p del mutex" );
		return &result;
	}
	result.cod_ret = 1;
	sprintf( result.retorno_u.id_cliente, "%d", idCliente );
	idCliente++;
	if( v(idMutex) == -1 ){
		result.cod_ret = 2;
		strcpy( result.retorno_u.msj_error, "Error al hacer v del mutex" );
		return &result;
	}
	return &result;
}
