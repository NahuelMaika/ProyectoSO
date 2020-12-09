#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

/*
 * Todos los ejecutables de los comandos deben estar en la misma carpeta de este mismo archivo "miniShell.c"
 */
int main(){
	char comando[1000000];
	char cwd[PATH_MAX];
	char argumento1[1000000] = "";
	char argumento2[1000000] = "";
	char argumento3[1000000] = "";
	char * coman2;

	while(1){
		printf("%s $ ", getcwd(cwd, sizeof(cwd)) );
		gets(comando);
		fflush(stdin);
		coman2 = strtok(comando, " ");
		
		if(coman2 != NULL)
			strcpy(argumento1, coman2);
		
		coman2 = strtok(NULL, " ");
		if(coman2 != NULL)
			strcpy(argumento2, coman2);

		coman2 = strtok(NULL, " ");
		if(coman2 != NULL)
			strcpy(argumento3, coman2);
		
		char *const argv2[] = {argumento1, argumento2, argumento3,NULL};
		
		char *direccion = getcwd(cwd, sizeof(cwd));
		strcat(direccion,"/");
		strcat(direccion, argumento1);
		
		int pid = fork();
		if(pid > 0){
			wait(NULL);
		}else 
			if(pid == 0){
				execv(direccion, argv2);
				printf("Error ingrese un comando valido - para obtener ayuda ingrese 'ayuda' \n");
			}else printf("Error al crear el proceso");
	}
}
