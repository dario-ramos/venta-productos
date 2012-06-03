#

MYLIB= enviar.o recibir.o tcpopact.o
LIBS	=
CFLAGS	= -O -g -D_SLAX_ -I/home/tomas/Documents/Distribuidos/trunk/servidor_ids_clientes

# Internet stream version (TCP protocol).
#

all:	 cliente puerto_entrada puerto_salida

cliente: cliente.cpp venta_tickets.h Cliente.h Comunicacion.h Comunicacion.cpp mensajes.h servidor_ids_clientes_clnt.c servidor_ids_clientes_xdr.c $(MYLIB)
	c++ $(CFLAGS) -o $@ cliente.cpp Comunicacion.cpp servidor_ids_clientes_clnt.c servidor_ids_clientes_xdr.c $(MYLIB)

puerto_entrada: puerto_entrada.cpp  $(MYLIB)
	c++ $(CFLAGS) -o $@ puerto_entrada.cpp  $(MYLIB)

puerto_salida: puerto_salida.cpp  $(MYLIB)
	c++ $(CFLAGS) -o $@ puerto_salida.cpp $(MYLIB)

enviar.o: enviar.cpp inet.h
	g++ $(CFLAGS) -c -o $@ enviar.cpp

recibir.o: recibir.cpp inet.h
	g++ $(CFLAGS) -c -o $@ recibir.cpp

tcpopact.o: recibir.cpp inet.h
	g++ $(CFLAGS) -c -o $@ tcpopact.cpp


clean:
	rm -f *.o cliente puerto_entrada puerto_salida


