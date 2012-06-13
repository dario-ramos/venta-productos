#include "ServidorIds.h"

ServidorIds::ServidorIds() : file(ARCHIVO , fstream::in | fstream::out | fstream::binary), error_(false){
	if(!file.is_open()){
		if(!generar_archivo()){
			perror("servidor_ids: error al generar el archivo de ids. ");
			file.close();
			sprintf(error_msg_, "No se pudo generar el archivo de ids.\n");
			printf("No hay mas ids libres.\n");
		}
		file.open(ARCHIVO, fstream::in | fstream::out | fstream::binary);
	}
}

ServidorIds::~ServidorIds(){
	file.close();
	if(file.fail()){
		perror("ServidorIds: error al cerrar el archivo.");
	}
}

int ServidorIds::pedirId(){
	int pos;
	int id;
	file.seekg (0, ios::beg);
	file.read((char *)&pos, sizeof(int));
	//si hay ids libres, devuelvo la siguiente
	if(pos <= CANT_CLIENTES){
		file.seekg(pos*sizeof(int));
		file.read((char *)&id, sizeof(int));
		pos++;
		file.seekg (0, ios::beg);
		file.write((char *)&pos, sizeof(int));
	}else{
		error_ = true;
		sprintf(error_msg_, "No hay mas ids libres.\n");
	}
	return id;
}

bool ServidorIds::devolverId(int id){
	int pos;
	file.seekg (0, ios::beg);
	file.read((char *)&pos, sizeof(int));
	//si hay ids libres, devuelvo la siguiente
	if(pos > 1){
		pos--;
		file.seekg(pos*sizeof(int));
		file.write((char *)&id, sizeof(int));
		file.seekg (0, ios::beg);
		file.write((char *)&pos, sizeof(int));
	}else{
		sprintf(error_msg_, "Se intento devolver id existente.\n");
		error_ = true;
		return false;
	}
	return true;
}

bool ServidorIds::generar_archivo(){
	int i = 1;
	file.open(ARCHIVO, fstream::out | fstream::binary);
	file.seekp(0, ios::beg);
	file.write((char *)&i, sizeof(i));
	for(; i<= CANT_CLIENTES; i++){
		file.write((char *)&i, sizeof(int));
	}
	file.close();
	if(file.bad()){
		error_ = true;
		sprintf(error_msg_, "Error al generar el archivo de ids");
	}
	return !file.bad();
}
