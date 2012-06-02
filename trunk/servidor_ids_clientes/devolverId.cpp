#include <cstdlib>
#include <cstdio>
#include "ServidorIds.h"
int
main (int argc, char ** argv){
	ServidorIds servidor_ids;
	int id = atoi(argv[1]);
	if(servidor_ids){
		servidor_ids.devolverId(id);
	}
	return 0;
}
