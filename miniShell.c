#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#include <dirent.h>
#define _XOPEN_SOURCE 700

void crearDirectorio(char * nombre){
	
		if(mkdir(nombre, S_IRUSR) < 0){
			perror("Error al crear directorio");
		}
}

void eliminarDir(char * nombre){
		if(rmdir(nombre) < 0){
			perror("Error al eliminar el archivo");
		}
}

/*
 * Crea un archivo con permisos 0777 por defecto
 */
void crearArchivo(char * nombre){
		if (creat(nombre, 0777) < 0){
			perror("Error al crear archivo");
		}
}

void getContenidoDir(){	
	printf("\n");
	DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d){
        while ((dir = readdir(d)) != NULL)
            printf("%s\n", dir->d_name);
        closedir(d);
    }
}

void getContenidoFile(char* nombre){
    FILE * fPtr;
    char ch;

	// cambiar a la ruta del archivo que le pasen por parametro
    fPtr = fopen(nombre, "r");

    if(fPtr == NULL){
        printf("No se pudo abrir el archivo.\n");
        return 0;
    }

    do{
        //lee un caracter del archivo y lo imprime
        ch = fgetc(fPtr);
        putchar(ch);
    } while(ch != EOF);

    fclose(fPtr);
}

void gethelp(){
	printf("Los comandos posibles son: \n");
	printf("mkdir - crea directorios \n");
	printf("SYNOPSYS \n");
	
	printf("Crear directorio mkdir 'nombre del nuevo directorio' \n");
	printf("Eliminar un directorio rmdir 'nombre del dir a borrar' \n");
	printf("Crear un archivo mkfil 'nombre del archivo' \n");
	printf("Listar el contenido de un directorio showd 'nombre del dir' \n");
	printf("Mostrar el contenido de un archivo showf 'nombre del archivo' \n");
	printf("Mostrar una ayuda con los comando posibles ayuda \n");
	printf("Modificar los permisos de un archivo modle 'nombre del archivo' 'nuevos permisos' \n");

}

/*
char* leerComando(char * com){
	
}*/

void cambiarPermisos(char *nombreArchivo, mode_t per){
	chmod(nombreArchivo, per);
}

int main(){
	
/* Permisos 
 * 0000 is ---------
 * 0666 is rw-rw-rw-
 * 0777 is rwxrwxrwx
 * 0700 is rwx------
 * 0100 is --x------
 * 0001 is --------x
 * 0002 is -------w-
 * 0003 is -------wx
 * 0004 is ------r--
 * 0005 is ------r-x
 * 0006 is ------rw-
 * 0007 is ------rwx 
 */
	
	char comando[1000000];
	char cm[20] = "";
	char cwd[PATH_MAX];
	char mkdir[] = "mkdir";
	char rmdir[] = "rmdir";
	char mkfil[] = "mkfil";
	char showd[] = "showd";
	char showf[] = "showf";
	char ayuda[] = "ayuda";
	char salir[] = "salir";


	while(1){
		
		printf("%s $ ", getcwd(cwd, sizeof(cwd)) );
		gets(comando);
		fflush(stdin);
		printf("comando %s\n", comando);
		
		char *coman2 = strtok(comando, " ");
		printf("Coman2 %s\n", coman2);
		strcpy(cm, coman2);
			
		if(strcmp(cm, mkdir) == 0){
			coman2 = strtok(NULL, " ");
			crearDirectorio(coman2);
		}else
			if(strcmp(cm, rmdir) == 0){
				printf("BORRO DIRECTORIO\n");
				coman2 = strtok(NULL, " ");
				eliminarDir(coman2);
			}else
				if(strcmp(cm, mkfil) == 0){
					printf("CREO ARCHIVO\n");
					coman2 = strtok(NULL, " ");
					crearArchivo(coman2);
				}else
					if(strcmp(cm, showd) == 0){
						printf("MUESTRO CONTENIDO DEL DIRECTORIO\n");
						getContenidoDir();
					}else
						if(strcmp(cm, showf) == 0){
							printf("MUESTRO CONTENIDO DE UN ARCHIVO\n");
							coman2 = strtok(NULL, " ");
							getContenidoFile(coman2);
						}else
							if(strcmp(cm, ayuda) == 0){
								gethelp();
							}else
								if(strcmp(cm, salir) == 0){
									printf("SALGO\n");
									break;
								}
								else
									printf("Comando no reconocido para obtener ayuda ingrese 'ayuda'\n");
		
      	//crearArchivo("archivoNuevo", S_IRWXU);
	}

	exit(0);
}

