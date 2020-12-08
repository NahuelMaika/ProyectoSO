#include <unistd.h>
#include <stdio.h>

/*
 * Pasar la ruta del directorio a eliminar
 */
int main(int argc, char *argv[]){	
	if(rmdir(argv[1]) < 0){
		perror("Error al eliminar el directorio");
	}
	return 0;
}

