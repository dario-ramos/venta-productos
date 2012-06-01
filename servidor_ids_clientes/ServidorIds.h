#ifndef SERVIDOR_IDS_H
#define SERVIDOR_IDS_H
#include <fstream>

#define ARCHIVO "/home/tomas/Documents/Distribuidos/trunk/servidor_ids_clientes/ids_servidor_rpc"
#define CANT_CLIENTES 10

using namespace std;
class ServidorIds{
public:
	ServidorIds();
	~ServidorIds();
	int pedirId();
	bool devolverId(int id);
	char * error_msg(){return error_msg_;}
	operator bool() const {return error_;}
private:
	fstream file;
	bool generar_archivo();
	bool error_;
	char error_msg_[200];
};

#endif
