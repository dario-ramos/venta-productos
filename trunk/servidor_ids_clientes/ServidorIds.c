#include "ServidorIds.h"

int ServidorIds_GenerarArchivo( ServidorIds* pThis ){
	int i = 1;
	pThis->archivoIds = fopen( ARCHIVO, "w+b" );
	fseek( pThis->archivoIds, 0, SEEK_SET );
	fwrite( (char*)&i, sizeof(int), 1, pThis->archivoIds );
	for( ; i<= MAX_CANT_CLIENTES; i++ ){
		fwrite( (char*)&i, sizeof(int), 1, pThis->archivoIds );
	}
	fclose( pThis->archivoIds );
	if( ferror( pThis->archivoIds ) ){
		pThis->huboError = 1;
		sprintf( pThis->msjError, "Error al generar el archivo de ids" );
	}
	return !ferror( pThis->archivoIds );
}


void ServidorIds_Conectar( ServidorIds* pThis ){
	pThis->archivoIds = fopen( ARCHIVO, "r+b" );
	pThis->huboError = 0;
	if( pThis->archivoIds == NULL ){
		if( !ServidorIds_GenerarArchivo( pThis ) ){
			perror( "servidor_ids: error al generar el archivo de ids. " );
			fclose( pThis->archivoIds );
			sprintf( pThis->msjError, "No se pudo generar el archivo de ids.\n" );
			printf("No hay mas ids libres.\n");
		}
		pThis->archivoIds = fopen( ARCHIVO, "r+b" );
	}
}

void ServidorIds_Desconectar( ServidorIds* pThis ){
	fclose( pThis->archivoIds );
	if( ferror(pThis->archivoIds) ){
		perror("ServidorIds: error al cerrar el archivo.");
	}
}

int ServidorIds_PedirId( ServidorIds* pThis ){
	int pos;
	int id;
	fseek( pThis->archivoIds, 0, SEEK_SET );
	fread( (char *)&pos, sizeof(int), 1, pThis->archivoIds );
	/* Si hay ids libres, devuelvo el siguiente */
	if( pos <= MAX_CANT_CLIENTES ){
		fseek( pThis->archivoIds, pos*sizeof(int), SEEK_SET );
		fread( (char*)&id, sizeof(int), 1, pThis->archivoIds );
		pos++;
		fseek( pThis->archivoIds, 0, SEEK_SET );
		fwrite( (char*)&pos, sizeof(int), 1, pThis->archivoIds );
	}else{
		pThis->huboError = 1;
		sprintf( pThis->msjError, "No hay mas ids libres.\n" );
	}
	return id;
}

int ServidorIds_DevolverId( ServidorIds* pThis, int id ){
	int pos;
	fseek( pThis->archivoIds, 0, SEEK_SET );
	fread( (char *)&pos, sizeof(int), 1, pThis->archivoIds );
	/* Si hay ids libres, devuelvo el siguiente */
	if(pos > 1){
		pos--;
		fseek( pThis->archivoIds, pos*sizeof(int), SEEK_SET );
		fwrite( (char *)&id, sizeof(int), 1, pThis->archivoIds );
		fseek( pThis->archivoIds, 0, SEEK_SET );
		fwrite( (char *)&pos, sizeof(int), 1, pThis->archivoIds );
	}else{
		sprintf( pThis->msjError, "Se intento devolver id existente.\n");
		pThis->huboError = 1;
		return 1;
	}
	return 0;
}

