#include <cstdio>
#include "ServidorIds.h"
int
main (void){
	ServidorIds servidor_ids;
	if(servidor_ids){
		int id = servidor_ids.pedirId();
		if(servidor_ids){
		printf("ID = %i\n", id);
		}
	}
	return 0;
}

