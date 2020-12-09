#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	printf("Los comandos posibles son: \n");
	printf("mkdir [nombre del directorio nuevo] crea directorios \n\n");	
	printf("rmdir [nombre del directorio a borrar] Eliminar un directorio \n\n");
	printf("mkfil [nombre del archivo] Crear un archivo \n\n");
	printf("showd [ubicacion del directorio] Listar el contenido de un directorio. Se debe pasar junto con su ruta \n\n");
	printf("showf [nombre del archivo] Muestra el contenido de un archivo el cual debe estar en la misma carpeta donde se ejecuta el miniShell \n o de lo contrario deberaindicar su ruta junto al nombre del archivo\n\n");
	printf("modle [nombre del archivo] [nuevos permisos] Modifica los permisos de un archivo, se debe pasar los nuevos permisos en formato numerico\nel archivo debera estar en la misma carpeta donde se ejecuta el miniShell \n\n");
	printf("ayuda Muestra los comando posibles \n\n");
	return 0;
}
