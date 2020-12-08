#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

/*
 * Muestra por consola el contenido de la carpeta ubicada en la ruta pasados por parametro argv[1]
 */
int main(int argc, char *argv[]){
    DIR *d;
    struct dirent *dir;
    d = opendir(argv[1]);
    if (d){
        while ((dir = readdir(d)) != NULL)
            printf("%s\n", dir->d_name);
        closedir(d);
    }
	return 0;
}

