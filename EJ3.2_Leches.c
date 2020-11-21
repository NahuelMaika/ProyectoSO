#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define cant_procesos 3
#define FILE_PATH "/home/pi/Desktop/Proyecto"
#define Tipo1 1L //leche
#define Tipo2 2L //comprarleche
#define Tipo3 3L

struct message{
	long type;
};
typedef struct message tMessage;

#define SIZE_MSG sizeof(struct message) - sizeof(long)

void report_and_exit(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

void comportamiento_comp(int * data,int queueId){
	int *aux=data;
	int i=*aux;
	tMessage rcv1,snd1,snd2;
	rcv1.type=-5;
	sleep(1);
	while(1){	
		msgrcv(queueId,&rcv1,SIZE_MSG,Tipo1,IPC_NOWAIT);
		printf("(%i): rcv1 %li\n",i,rcv1.type);
		if(rcv1.type == Tipo1){//Hay leche
			printf("(Compañero %i): Miro el refrigerador y hay leche\n",i);
			printf("(Compañero %i): Consumo una botella de leche\n",i);	
			snd2.type = Tipo2;
			msgsnd(queueId,&snd2,SIZE_MSG,0);//Mando msj de que hay que ir a comprar*/
			rcv1.type=-4;
			sleep(1);
		}else{//No hay leche
			sleep(1);
			printf("(Compañero %i): Miro el refrigerador y no hay leche\n",i);
			msgrcv(queueId,&rcv1,SIZE_MSG,Tipo2,IPC_NOWAIT);
			printf("(%i): rcv1 %li\n",i,rcv1.type);
			if(rcv1.type == Tipo2){//Voy a comprar
				printf("(Compañero %i): Voy al supermercado\n",i);
				sleep(1);
				printf("(Compañero %i): Llego al supermercado\n",i);
				sleep(1);
				printf("(Compañero %i): Compro leche\n",i);
				sleep(1);
				printf("(Compañero %i): Llegue a la casa y guarde la leche\n",i);
				snd1.type = Tipo1;
				msgsnd(queueId,&snd1,SIZE_MSG,0);
				rcv1.type=-3;
				sleep(1);
			}else{
				printf("(Compañero %i): Alguien ya fue a comprar leche\n",i);
				printf("(Compañero %i): Voy a hacer otras cosas\n",i);
				sleep(3);
			}
				
		}
	}
	
	exit(0);
}

int main(){
	
	pid_t pid=getpid();
	key_t key = ftok(FILE_PATH, 1);
	if(key < 0){
		report_and_exit("ftok");
	}

	int queueId = msgget(key,0666 | IPC_CREAT);
	if(queueId < 0){
		report_and_exit("msgget");
	}
	
	int cont;
	for(int i=0;i<cant_procesos;i++){
		if(pid>0){
			cont=i;
			pid=fork();	
		}
	}
	if(pid<0)
		exit(1);
	
	if(pid == 0){
		comportamiento_comp(&cont,queueId);
	}
	
	if(pid>0){
		tMessage men;
		men.type=Tipo1;
		msgsnd(queueId,&men,SIZE_MSG,0);
		printf("Soy el proceso padre y mi pid es: %i\n",getpid());
		for(int i=0;i<cant_procesos;i++)
			wait(NULL);
			
		msgctl(queueId,IPC_RMID,NULL);
	}
	
	exit(0);
}
