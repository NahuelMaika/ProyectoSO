#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define Caso1 4
#define Caso2 5
#define Caso3 6

#define cant_hilos 2

#define Caso1_A 2
#define Caso1_B 2

#define Caso2_A 2
#define Caso2_B 1
#define Caso2_C 2

#define Caso3_A 2
#define Caso3_B 2
#define Caso3_C 2

#define TParcial 0
#define TTotal 1

#define Verificacion 0
#define Reparacion 1

#define ReparacionLlanta 0
#define RotyBalanceo 1

#define Termine 0

char *colores[4]= {"Negro","Blanco","Azul","Rojo"};

struct MA{
	int color;
	int trabajo;
};
typedef struct MA MsjA;
typedef struct MA *MsjA1;


struct MC{
	int trabajo;
	int llantas;	
};
typedef struct MC MsjC;
typedef struct MC *MsjC1;

struct MCC{
	int cantidad;
	MsjC mensaje;
};
typedef struct MCC * MsjCC;
struct MCB{
	int cantidad;
	int trabajo;
};
typedef struct MCB *MsjCB;

struct MCA{
	int cantidad;
	MsjA mensaje;
};
typedef struct MCA *MsjCA;

sem_t semA,semB,semC,coordA,coordB,coordC;

int pipeA[2];
int pipeB[2];
int pipeC[2];
int pipeFinalA[2];
int pipeFinalB[2];
int pipeFinalC[2];

void * tareaA(void* data){
	MsjA1 menA=data;
	
	while(1){
		sem_wait(&semA);
		if(menA->trabajo == TParcial){
				printf("(A): Voy a hacer una tarea parcial con el color %s\n",colores[menA->color]);
				sleep(1);
				printf("(A): Termine de hacer el trabajo parcial con el color %s\n",colores[menA->color]);
		}else{ if (menA->trabajo == TTotal){
						printf("(A): Voy a hacer una tarea total con el color %s\n",colores[menA->color]);
						sleep(3);
						printf("(A): Termine de hacer el trabajo total con el color %s\n",colores[menA->color]);
			 }
		}
		
		sem_post(&coordA);
	}
}

void coord_A(){
	close(pipeB[0]);
	close(pipeC[0]);
	close(pipeA[1]);
	close(pipeB[1]);
	close(pipeC[1]);
	close(pipeFinalB[1]);
	close(pipeFinalC[1]);
	close(pipeFinalA[0]);
	close(pipeFinalB[0]);
	close(pipeFinalC[0]);
	
	MsjCA menCA1 =malloc(sizeof(struct MCA));
	MsjA1 menA1 = malloc(sizeof(struct MA));
	MsjA1 menA2 = malloc(sizeof(struct MA));
	pthread_t hilos[cant_hilos];
	pthread_create(&hilos[0],NULL,tareaA,menA1);
	pthread_create(&hilos[1],NULL,tareaA,menA2);
		
	int cant_tareas;
	int *res= malloc(sizeof(int));
	*res=-1;
	while(1){
		read(pipeA[0],menCA1,sizeof(struct MCA));
		cant_tareas = menCA1->cantidad;
		printf("Cant tareas de A recibidas: %i\n",cant_tareas);
		
		menA1->color=menCA1->mensaje.color;
		menA1->trabajo=menCA1->mensaje.trabajo;
		sem_post(&semA);	
		
		for(int i=1;i<cant_tareas;i++){ //DEBERIA HACER UN DO-WHILE PERO NO PUEDO PUES AMBOS HILOS TIENEN DISTINTOS STRUCTS
			read(pipeA[0],menCA1,sizeof(struct MCA));
			menA2->color=menCA1->mensaje.color;
			menA2->trabajo=menCA1->mensaje.trabajo;
			sem_post(&semA);	
		}
			
		for(int i=0; i<cant_tareas;i++){
			sem_wait(&coordA);
			printf("Termine una Tarea A\n");
			*res=Termine;
			write(pipeFinalA[1],res,sizeof(int));
			*res=-1;
		}
	}
}

void crearMsjA(int cant){
	
	for(int i=0;i<cant;i++){
		MsjCA mensajeCA = malloc(sizeof(struct MCA));
		MsjA *mA = malloc(sizeof(struct MA));
		mA->color=rand()%4;
		mA->trabajo=rand()%2;
		mensajeCA->cantidad = cant;
		mensajeCA->mensaje = *mA;
		write(pipeA[1],mensajeCA,sizeof(struct MCA));	
	}
}



void * tareaB(void* data){
	int *aux;
	aux = (int *) data;
	int menB;
	
	while(1){
		sem_wait(&semB);
		menB = *aux;
		if(menB == Verificacion){
				printf("(B): Voy a hacer una verificacion\n");
				sleep(1);
				printf("(B): Termine de hacer el trabajo de verificacion\n");
			}else if (menB == Reparacion){
						printf("(B): Voy a hacer una reparacion\n");
						sleep(2);
						printf("(B): Termine de hacer el trabajo de reparacion\n");
			}
		int *ret;
		ret=malloc(sizeof(int));
		*ret=Termine;
		write(pipeFinalB[1],ret,sizeof(int));
		sem_post(&coordB);
	}
}

void coord_B(){
	close(pipeA[0]);
	close(pipeC[0]);
	close(pipeA[1]);
	close(pipeB[1]);
	close(pipeC[1]);
	close(pipeFinalA[1]);
	close(pipeFinalC[1]);
	close(pipeFinalA[0]);
	close(pipeFinalB[0]);
	close(pipeFinalC[0]);
	
	MsjCB menCB1 = malloc(sizeof(struct MCB));
	int *mensajeB1;
	mensajeB1= malloc(sizeof(int));
	int *mensajeB2;
	mensajeB2= malloc(sizeof(int));
	pthread_t hilos[cant_hilos]; 
	pthread_create(&hilos[0],NULL,tareaB,mensajeB1);
	pthread_create(&hilos[1],NULL,tareaB,mensajeB2);
	
	int cant_tareas;
	int *res= malloc(sizeof(int));
	*res=-1;
	while(1){
		read(pipeB[0],menCB1,sizeof(struct MCB));
		cant_tareas = menCB1->cantidad;
		printf("Cant tareas de B recibidas: %i\n",cant_tareas);
		*mensajeB1=menCB1->trabajo;
		sem_post(&semB);
		
		for(int i=1;i<cant_tareas;i++){
			read(pipeB[0],menCB1,sizeof(struct MCB));
			*mensajeB2=menCB1->trabajo;
			sem_post(&semB);
		}
			
		for(int i=0; i<cant_tareas;i++){
			sem_wait(&coordB);
			printf("Termine una Tarea B\n");
			*res=Termine;
			write(pipeFinalB[1],res,sizeof(int));
			*res=-1;
		}
	}
	
}

void crearMsjB(int cant){
	for(int i=0;i<cant;i++){
		MsjCB mensajeCB1 = malloc(sizeof(struct MCB));
		mensajeCB1->cantidad = cant;
		mensajeCB1->trabajo = rand()%2;
		write(pipeB[1],mensajeCB1,sizeof(struct MCB));
	}
}



void * tareaC(void* data){
	MsjC1 menC=data;
	
	while(1){
		sem_wait(&semC);
		if(menC->trabajo == ReparacionLlanta){
				printf("(C): Voy a hacer una reparacion de %i llantas\n",menC->llantas);
				sleep(menC->llantas);
				printf("(C): Termine de reparar las llantas %i del vehiculo\n",menC->llantas);
			}else if (menC->trabajo == RotyBalanceo){
						printf("(C): Voy a hacer un trabajo de rotacion y balanceo\n");
						sleep(3);
						printf("(C): Termine de hacer el trabajo de rotacion y balanceo \n");
			}
		
		
		sem_post(&coordC);
	}
}

void coord_C(){
	close(pipeA[0]);
	close(pipeB[0]);
	close(pipeA[1]);
	close(pipeB[1]);
	close(pipeC[1]);
	close(pipeFinalA[1]);
	close(pipeFinalB[1]);
	close(pipeFinalA[0]);
	close(pipeFinalB[0]);
	close(pipeFinalC[0]);
	
	MsjCC menCC1 =malloc(sizeof(struct MCC));
	MsjC1 menC1 = malloc(sizeof(struct MC));
	MsjC1 menC2 = malloc(sizeof(struct MC));
	pthread_t hilos[cant_hilos];
	pthread_create(&hilos[0],NULL,tareaC,menC1);
	pthread_create(&hilos[1],NULL,tareaC,menC2);
		
	int cant_tareas;
	int *res= malloc(sizeof(int));
	*res=-1;
	while(1){
		read(pipeC[0],menCC1,sizeof(struct MCC));
		cant_tareas = menCC1->cantidad;
		printf("Cant tareas de C recibidas: %i\n",cant_tareas);
		
		menC1->trabajo=menCC1->mensaje.trabajo;
		menC1->llantas=menCC1->mensaje.llantas;
		sem_post(&semC);
			
		for(int i=1;i<cant_tareas;i++){
			read(pipeC[0],menCC1,sizeof(struct MCC));
			menC2->trabajo=menCC1->mensaje.trabajo;
			menC2->llantas=menCC1->mensaje.llantas;
			sem_post(&semC);		
		}
		
			
		for(int i=0; i<cant_tareas;i++){
			sem_wait(&coordC);
			printf("Termine una Tarea C\n");
			*res=Termine;
			write(pipeFinalC[1],res,sizeof(int));
			*res=-1;
		}
	}
}

void crearMsjC(int cant){
	for(int i=0;i<cant;i++){
		MsjCC mensajeCC1 = malloc(sizeof(struct MCC));
		MsjC *mC1 = malloc(sizeof(struct MC));
		mC1->trabajo=rand()%2;
		if(mC1->trabajo == ReparacionLlanta)
			mC1->llantas=(rand()%4) + 1;
		if(mC1->trabajo == RotyBalanceo)
			mC1->llantas=0;
		mensajeCC1->cantidad = cant;
		mensajeCC1->mensaje = *mC1;
		write(pipeC[1],mensajeCC1,sizeof(struct MCC));
	}
}




int main(){
	srand(time(NULL));
	
	pipe(pipeA);
	pipe(pipeB);
	pipe(pipeC);
	pipe(pipeFinalA);
	pipe(pipeFinalB);
	pipe(pipeFinalC);
	
	pid_t pid;
	
	pid=fork();
	if(pid>0){
		pid=fork();
		if(pid>0){
			pid=fork();
			if(pid==0){
				coord_C();
			}
		}else if(pid == 0){
				  coord_B();
		}
	}else if(pid == 0){
			coord_A();
	}
	
	if(pid<0)
		exit(1);
				
	if(pid>0){
		close(pipeA[0]);
		close(pipeB[0]);
		close(pipeC[0]);
		close(pipeFinalA[1]);
		close(pipeFinalB[1]);
		close(pipeFinalC[1]);
		int num=1;
		int a,b,c;
		
		int *res= malloc(sizeof(int));
		*res=-1;
		while(1){
			num=rand()%3 + 4;
			printf("------------------------\n");
			printf("VAMOS A HACER EL CASO %i\n",num % 4 + 1);
			printf("------------------------\n");
			sleep(1);
			a=0;
			b=0;
			c=0;
				
			if(num == Caso1){
				a=2;
				b=2;
		
				crearMsjA(a);
				crearMsjB(b);
			}
			if(num == Caso2){
				a=2;
				b=1;
				c=2;
				
				crearMsjA(a);
				crearMsjB(b);
				crearMsjC(c);				
			}
			
			if(num == Caso3){
				a=2;
				b=2;
				c=2;
				
				crearMsjA(a);
				crearMsjB(b);
				crearMsjC(c);	
			}
			if(num == 2){
				a=2;
				c=2;
				crearMsjA(a);
				crearMsjC(c);
			
			}
			for(int i=0;i<a;i++)
				read(pipeFinalA[0],res,sizeof(int));
			for(int i=0;i<b;i++)
				read(pipeFinalB[0],res,sizeof(int));
			for(int i=0;i<c;i++)
				read(pipeFinalC[0],res,sizeof(int));
				
			
		}
	}
		
}
