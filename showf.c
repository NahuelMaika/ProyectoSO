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
 * 
 */
int main(int argc, char *argv[]){
    FILE * fPtr;
    char ch;

    //Abre el archivo en la ruta indicada
    fPtr = fopen(argv[1], "r");

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
    return 0;
}
