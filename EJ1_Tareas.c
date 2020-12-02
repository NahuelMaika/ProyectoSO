#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#define Caso1 4
#define Caso2 5
#define Caso3 6

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


int pipeA[2];
int pipeB[2];
int pipeC[2];
int pipeFinalA[2];
int pipeFinalB[2];
int pipeFinalC[2];

void * tareaA(void* data){
	MsjA1 menA=data;
	
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
	int *ret;
	ret=malloc(sizeof(int));
	*ret=Termine;
	write(pipeFinalA[1],ret,sizeof(int));
	pthread_exit(0);
}

void * tareaB(void* data){
	int *aux;
	aux = (int *) data;
	int menB = *aux;
	
	
	
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
	pthread_exit(0);
}

void * tareaC(void* data){
	MsjC1 menC=data;
	if(menC->trabajo == ReparacionLlanta){
			printf("(C): Voy a hacer una reparacion de %i llantas\n",menC->llantas);
			sleep(menC->llantas);
			printf("(C): Termine de reparar las llantas %i del vehiculo\n",menC->llantas);
		}else if (menC->trabajo == RotyBalanceo){
					printf("(C): Voy a hacer un trabajo de rotacion y balanceo\n");
					sleep(3);
					printf("(C): Termine de hacer el trabajo de rotacion y balanceo \n");
		}
	
	int *ret;
	ret=malloc(sizeof(int));
	*ret=Termine;
	write(pipeFinalC[1],ret,sizeof(int));
	pthread_exit(0);
}

void coord_A(){
    //close(pipeA[1]);
	
	MsjCA menCA1 =malloc(sizeof(struct MCA));
	MsjA1 menA1 = malloc(sizeof(struct MA));
	MsjA1 menA2 = malloc(sizeof(struct MA));
	int cant_tareas;
	int *res= malloc(sizeof(int));
	*res=-1;
	while(1){
		read(pipeA[0],menCA1,sizeof(struct MCA));
		cant_tareas = menCA1->cantidad;
		menA1->color=menCA1->mensaje.color;
		menA1->trabajo=menCA1->mensaje.trabajo;
		printf("Cant tareas de A recibidas: %i\n",cant_tareas);

		pthread_t hilos[cant_tareas]; 
		
		pthread_create(&hilos[0],NULL,tareaA,menA1);
		
		for(int i=1;i<cant_tareas;i++){
			read(pipeA[0],menCA1,sizeof(struct MCA));
			menA2->color=menCA1->mensaje.color;
			menA2->trabajo=menCA1->mensaje.trabajo;
			pthread_create(&hilos[i],NULL,tareaA,menA2);	
		}
		
		
			
		for(int i=0; i<cant_tareas;i++){
			read(pipeFinalA[0],res,sizeof(int));
			printf("Termine una Tarea A\n");
		}
	}
}

void coord_B(){
	MsjCB menCB1 = malloc(sizeof(struct MCB));
	int cant_tareas;
	int *mensajeB1;
	mensajeB1= malloc(sizeof(int));
	int *mensajeB2;
	mensajeB2= malloc(sizeof(int));
	int *res= malloc(sizeof(int));
	*res=-1;
	while(1){
		read(pipeB[0],menCB1,sizeof(struct MCB));
		cant_tareas = menCB1->cantidad;
		printf("Cant tareas de B recibidas: %i\n",cant_tareas);
		*mensajeB1=menCB1->trabajo;
		
		pthread_t hilos[cant_tareas]; 
		
		pthread_create(&hilos[0],NULL,tareaB,mensajeB1);
		
		for(int i=1;i<cant_tareas;i++){
			read(pipeB[0],menCB1,sizeof(struct MCB));
			*mensajeB2=menCB1->trabajo;
			pthread_create(&hilos[i],NULL,tareaB,mensajeB2);	
		}
		
		
			
		for(int i=0; i<cant_tareas;i++){
			read(pipeFinalB[0],res,sizeof(int));
			printf("Termine una Tarea B\n");
		}
	}
	
}

void coord_C(){
	
	MsjCC menCC1 =malloc(sizeof(struct MCC));
	int cant_tareas;
	MsjC1 menC1 = malloc(sizeof(struct MC));
	MsjC1 menC2 = malloc(sizeof(struct MC));
	int *res= malloc(sizeof(int));
	*res=-1;
	while(1){
		read(pipeC[0],menCC1,sizeof(struct MCC));
		cant_tareas = menCC1->cantidad;
		menC1->trabajo=menCC1->mensaje.trabajo;
		menC1->llantas=menCC1->mensaje.llantas;
		printf("Cant tareas de C recibidas: %i\n",cant_tareas);
		
		pthread_t hilos[cant_tareas]; 
		
		pthread_create(&hilos[0],NULL,tareaC,menC1);
			
		for(int i=1;i<cant_tareas;i++){
			read(pipeC[0],menCC1,sizeof(struct MCC));
			cant_tareas=menCC1->cantidad;
			menC2->trabajo=menCC1->mensaje.trabajo;
			menC2->llantas=menCC1->mensaje.llantas;
			pthread_create(&hilos[i],NULL,tareaC,menC2);	
		}
		
			
		for(int i=0; i<cant_tareas;i++){
			read(pipeFinalC[0],res,sizeof(int));
			printf("Termine una Tarea C\n");
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

void crearMsjB(int cant){
	for(int i=0;i<cant;i++){
		MsjCB mensajeCB1 = malloc(sizeof(struct MCB));
		mensajeCB1->cantidad = cant;
		mensajeCB1->trabajo = rand()%2;
		write(pipeB[1],mensajeCB1,sizeof(struct MCB));
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
	int pidA,pidB,pidC;
	
	pid=fork();
	if(pid>0){
		pid=fork();
		if(pid>0){
			pid=fork();
			if(pid==0){
				pidC=getpid();
				coord_C();
			}
		}else if(pid == 0){
				  pidB=getpid();
				  coord_B();
		}
	}else if(pid == 0){
			pidA=getpid();
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
		
		while(num != 0){
			printf("Ingrese el numero 0 para salir, 4,5 o 6 para ejecutar tareas: ");
			scanf(" %i",&num);
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
			sleep(5);
		}
		
		kill(pidA,SIGKILL);
		kill(pidB,SIGKILL);
		kill(pidC,SIGKILL);
		printf("EL PADRE TERMINO\n");
	}
		
}
