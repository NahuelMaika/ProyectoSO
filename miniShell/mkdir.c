#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char *argv[]){
	if(mkdir(argv[1], 0777) < 0){
		perror("Error al crear directorio");
	}
	
	return 0;
}

