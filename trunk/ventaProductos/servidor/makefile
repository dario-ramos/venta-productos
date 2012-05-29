#

MYLIB= enviar.o recibir.o tcpoppas.o
LIBS	=
CFLAGS	= -O -g -Wall -D_SLAX_

# Internet stream version (TCP protocol).
#

all:	 servidor puerto_entrada puerto_salida finalizador vendedor

servidor: servidor.cpp venta_tickets.h semaforo.h $(MYLIB)
	c++ $(CFLAGS) -o $@ servidor.cpp $(MYLIB)

vendedor: main.cpp venta_tickets.h semaforo.h Vendedor.h Vendedor.cpp mensajes.h
	c++ $(CFLAGS) -o $@ main.cpp Vendedor.cpp 

puerto_entrada: puerto_entrada.cpp mensajes.h $(MYLIB)
	c++ $(CFLAGS) -o $@ puerto_entrada.cpp  $(MYLIB) $(LIBS)

puerto_salida: puerto_salida.cpp  $(MYLIB)
	c++ $(CFLAGS) -o $@ puerto_salida.cpp $(MYLIB) $(LIBS)

finalizador: finalizador.cpp venta_tickets.h semaforo.h
	c++ $(CFLAGS) -o $@ finalizador.cpp 

enviar.o: enviar.cpp inet.h
	g++ $(CFLAGS) -c -o $@ enviar.cpp

recibir.o: recibir.cpp inet.h
	g++ $(CFLAGS) -c -o $@ recibir.cpp

tcpoppas.o: recibir.cpp inet.h
	g++ $(CFLAGS) -c -o $@ tcpoppas.cpp


clean:
	rm -f *.o servidor puerto_entrada puerto_salida finalizador


