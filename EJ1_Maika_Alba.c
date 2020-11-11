#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


#define Caso1 4
#define Caso2 5
#define Caso3 6

#define TParcial 0
#define TTotal 1

#define Verificacion 0
#define Reparacion 1

#define ReparacionLlanta 0
#define RotyBalanceo 1

#define Termine 0

struct MA{
	char color[10];
	int trabajo;
};
typedef struct MA *MsjA;

int pA[2];
int pB[2];
int pC[2];
int pP[2];

void TareaA(){
	MsjA menA;
	close(pP[0]);
	close(pA[1]);
	while(1){
		read(pA[0],menA, sizeof(struct MA));
		printf("\nSoy la TareaA y lei mi pipe\n");
		printf("Me pasaron el color: %s \n",menA->color);
		
		if(menA->trabajo == TParcial){
			printf("Y voy a hacer una tarea parcial\n");
			sleep(1);
			printf("Termine de hacer el trabajo parcial de pintura sobre el vehiculo\n");
		}else if (menA->trabajo == TTotal){
					printf("Y voy a hacer una tarea total\n");
					sleep(3);
					printf("Termine de hacer el trabajo total de pintura sobre el vehiculo\n");
		}
		int ret=Termine;
		write(pP[1],&ret,sizeof(int));
	}			
}

void TareaB(){
	int menB;
	close(pP[0]);
	close(pB[1]);
	while(1){
		read(pB[0],&menB, sizeof(int));
		printf("\nSoy la TareaB y lei mi pipe\n");
		if(menB == Verificacion){
			printf("Voy a hacer una verificacion\n");
			sleep(1);
			printf("Termine de hacer el trabajo de verificacion sobre el vehiculo\n");
		}else if (menB == Reparacion){
					printf("Voy a hacer una reparacion\n");
					sleep(2);
					printf("Termine de hacer el trabajo de reparacion sobre el vehiculo\n");
		}
		int ret=Termine;
		write(pP[1],&ret,sizeof(int));
	}		
}

void TareaC(){
	int menC;
	close(pP[0]);
	close(pB[1]);
	while(1){
		read(pC[0],&menC, sizeof(int));
		printf("\nSoy la TareaC y lei mi pipe\n");
		if(menC == ReparacionLlanta){
			printf("Voy a hacer una reparacion de llantas\n");
			sleep(4);//Ya que un auto tiene 4 ruedas
			printf("Termine de reparar las llantas del vehiculo\n");
		}else if (menC == RotyBalanceo){
					printf("Voy a hacer una rotacion y balanceo\n");
					sleep(3);
					printf("Termine de hacer el trabajo de rotacion y balanceo sobre el vehiculo\n");
		}
		int ret=Termine;
		write(pP[1],&ret,sizeof(int));
	}		
}


int main(){
	pid_t pid;
	int pidA,pidB,pidC;
	pipe(pA);
	pipe(pB);
	pipe(pC);
	pipe(pP);
	
	
		
	
	
	pid=fork();
	if(pid > 0){
		pid=fork();
		if(pid>0){
			pid=fork();
			if(pid == 0){
				pidC=getpid();
				TareaC();
			}
		}else if(pid == 0){
			pidB=getpid();
			TareaB();
		}
	}else if(pid == 0){
		pidA=getpid();
		TareaA();
	}
	
	if(pid < 0)
		exit(1);
	
		
	if(pid > 0){
		close(pA[0]);
		close(pB[0]);
		close(pC[0]);
		close(pP[1]);
		
		MsjA mensajeA = malloc(sizeof(struct MA));
		int menPadre,mensajeB,mensajeC;
		
		int num=1;
		while(num != 0){
			char color[10];
			int *t;
			t=malloc(sizeof(int));
			*t=2;
			printf("Ingrese el numero 0 para salir, 4,5 o 6 para ejecutar tareas: ");
			scanf(" %i",&num);
	
			if(num == Caso1){
				printf("Ingrese un color: ");
				scanf("%s",color);
				strcpy(mensajeA->color,color);
				printf("Ingrese 0 para tarea parcial y 1 para tarea total: ");
				scanf("%i",t);
				mensajeA->trabajo = *t;
				write(pA[1],mensajeA,sizeof(struct MA));
				
				read(pP[0],&menPadre,sizeof(int));
				
				printf("Ingrese 0 para realizar una verificacion y 1 para una reparacion: ");
				scanf("%i",&mensajeB);
				write(pB[1],&mensajeB,sizeof(int));
				
				read(pP[0],&menPadre,sizeof(int));
				
				printf("Ingrese un color: ");
				scanf("%s",color);
				strcpy(mensajeA->color,color);
				printf("Ingrese 0 para tarea parcial y 1 para tarea total: ");
				scanf("%i",t);
				mensajeA->trabajo = *t;
				write(pA[1],mensajeA,sizeof(struct MA));
				
				read(pP[0],&menPadre,sizeof(int));
				
				printf("Ingrese 0 para realizar una verificacion y 1 para una reparacion: ");
				scanf(" %i",&mensajeB);
				write(pB[1],&mensajeB,sizeof(int));
				
				read(pP[0],&menPadre,sizeof(int));
				
			}else if(num == Caso2){
					printf("Ingrese un color: ");
					scanf("%s",color);
					strcpy(mensajeA->color,color);
					printf("Ingrese 0 para tarea parcial y 1 para tarea total: ");
					scanf("%i",t);
					mensajeA->trabajo = *t;
					write(pA[1],mensajeA,sizeof(struct MA));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una verificacion y 1 para una reparacion: ");
					scanf("%i",&mensajeB);
					write(pB[1],&mensajeB,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una reparacion de llantas y 1 para una rotacion y balanceo: ");
					scanf("%i",&mensajeC);
					write(pC[1],&mensajeC,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese un color: ");
					scanf("%s",color);
					strcpy(mensajeA->color,color);
					printf("Ingrese 0 para tarea parcial y 1 para tarea total: ");
					scanf("%i",t);
					mensajeA->trabajo = *t;
					write(pA[1],mensajeA,sizeof(struct MA));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una reparacion de llantas y 1 para una rotacion y balanceo: ");
					scanf("%i",&mensajeC);
					write(pC[1],&mensajeC,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));	
				
			}else if(num == Caso3){
					printf("Ingrese un color: ");
					scanf("%s",color);
					strcpy(mensajeA->color,color);
					printf("Ingrese 0 para tarea parcial y 1 para tarea total: ");
					scanf("%i",t);
					mensajeA->trabajo = *t;
					write(pA[1],mensajeA,sizeof(struct MA));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una verificacion y 1 para una reparacion: ");
					scanf("%i",&mensajeB);
					write(pB[1],&mensajeB,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una reparacion de llantas y 1 para una rotacion y balanceo: ");
					scanf("%i",&mensajeC);
					write(pC[1],&mensajeC,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese un color: ");
					scanf("%s",color);
					strcpy(mensajeA->color,color);
					printf("Ingrese 0 para tarea parcial y 1 para tarea total: ");
					scanf("%i",t);
					mensajeA->trabajo = *t;
					write(pA[1],mensajeA,sizeof(struct MA));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una verificacion y 1 para una reparacion: ");
					scanf(" %i",&mensajeB);
					write(pB[1],&mensajeB,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));
					
					printf("Ingrese 0 para realizar una reparacion de llantas y 1 para una rotacion y balanceo: ");
					scanf("%i",&mensajeC);
					write(pC[1],&mensajeC,sizeof(int));
					
					read(pP[0],&menPadre,sizeof(int));
			}
		}
	}
	
	kill(pidA,0);
	kill(pidB,0);
	kill(pidC,0);
	
	exit(0);
}
