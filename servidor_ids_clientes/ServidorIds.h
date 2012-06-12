#ifndef SERVIDOR_IDS_H
#define SERVIDOR_IDS_H

#include <stdio.h>

#define ARCHIVO "/home/tomas/Documents/Distribuidos/trunk/servidor_ids_clientes/ids_servidor_rpc.ids"
#define MAX_CANT_CLIENTES 10
#define SERVIDOR_IDS_MAX_LONG_MSJ_ERROR 200

typedef struct ServidorIds{
	FILE* archivoIds;
	int huboError;
	char msjError[SERVIDOR_IDS_MAX_LONG_MSJ_ERROR];
}ServidorIds;

void ServidorIds_Conectar( ServidorIds* pThis );

void ServidorIds_Desconectar( ServidorIds* pThis );

int ServidorIds_PedirId( ServidorIds* pThis );

int ServidorIds_DevolverId( ServidorIds* pThis, int id );

#endif
