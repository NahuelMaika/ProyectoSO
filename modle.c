#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	long int i = strtol(argv[2], 0, 8);
	
	if(chmod(argv[1], i) < 0)
		printf("Error al cambiar permisos de archivo");
	return 0;
}
