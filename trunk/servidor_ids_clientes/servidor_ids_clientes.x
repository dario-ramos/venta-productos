/*
 * servidor_ids_clientes.x: Entrega el id para un nuevo cliente de forma remota y segura
 */

/* 
 * estructura de la devolucion de resultados de la invocacion a la subrutina en RPC 
 * ESTE TIPO DE ESTRUCTURAS SON OBLIGATORIAS SI HAY QUE DEVOLVER RESULTADOS
 */

union retorno 
	switch (int cod_ret) {
		case 1: char id_cliente[800];
		case 2: char msj_error[800];
		default: void;
	};

program SERVIDOR_IDS_PROG {
	version SERVIDOR_IDS_VERS {
		retorno OBTENER_NUEVO_ID_CLIENTE(void) = 1;
	} = 1;
} = 0x20000099;
