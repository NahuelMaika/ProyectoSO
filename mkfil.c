#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * Crea un archivo con permisos 0777 por defecto
 * Se debe pasar el nombre del archivo junto con su ruta en argv[1]
 */
int main(int argc, char *argv[]){
	if (creat(argv[1], 0777) < 0){
		perror("Error al crear archivo");
	}
	return 0;
}
