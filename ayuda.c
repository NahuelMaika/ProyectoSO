#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	printf("Los comandos posibles son: \n");
	printf("mkdir - crea directorios \n");	
	printf("Crear directorio mkdir 'nombre del nuevo directorio' \n");
	printf("Eliminar un directorio rmdir 'nombre del dir a borrar' \n");
	printf("Crear un archivo mkfil 'nombre del archivo' \n");
	printf("Listar el contenido de un directorio showd 'nombre del dir' \n");
	printf("Mostrar el contenido de un archivo showf 'nombre del archivo' \n");
	printf("Mostrar una ayuda con los comando posibles ayuda \n");
	printf("Modificar los permisos de un archivo modle 'nombre del archivo' 'nuevos permisos' \n");
	return 0;
}
