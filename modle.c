#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	chmod(argv[1], atoi(argv[2]));
	return 0;
}
