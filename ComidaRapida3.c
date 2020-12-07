#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <pthread.h>
#include "constComidaRapida.h"
#include <time.h>

#define M 3	//Mesas 30
#define N 5	//Clientes 50
#define C 3 	//Cocineros


int abrirColaClientes(){
	key_t key = ftok(FILE_PATH, 1);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	return queueId;
}

int abrirColaComidas(){
	key_t key = ftok(FILE_PATH, 2);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	return queueId;
}


void * comer(void * arg){ 
	int * aux = (int*)arg;
	tMessage recibo, respuesta;
	int queue_cliente_Id=abrirColaClientes();

	msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Mesas, 0); // espero por mesa disponible
	printf("\nSoy el Cliente nro %i y me sente en una mesa\n", (*aux)+1);
	
	if(rand()%2){
		respuesta.type = Tipo_Vegetal_Pedido;
		msgsnd(queue_cliente_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
		printf("Soy cliente nro %i y pedi un plato vegetal\n", (*aux)+1);
		
		msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Vegetal_Listo, 0);//espero por mi pedido vegetal
		
		printf("Soy cliente nro %i me dieron mi plato vegetal y voy a comer\n", (*aux)+1);
		
	}else{
		respuesta.type = Tipo_Carne_Pedido;
		msgsnd(queue_cliente_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
		printf("Soy cliente nro %i y pedi un plato carne\n", (*aux)+1);
		
		msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Carne_Listo, 0);//espero por mi pedido carne
		
		printf("Soy cliente nro %i me dieron mi plato carne y voy a comer\n", (*aux)+1);
	}

	printf("Soy cliente nro %i y me voy\n", (*aux)+1);
	
	respuesta.type = Tipo_Limpiador;
	msgsnd(queue_cliente_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
	
	pthread_exit(0);
}

void * cocinar(void * arg){
	int * aux = (int*)arg;
	printf("SOY COCINERO %i\n",*aux);
	tMessage recibo, respuesta;
	int queue_comidas_Id = abrirColaComidas();
	int queue_cliente_Id=abrirColaClientes();
	
	
	while(1){
		msgrcv(queue_comidas_Id, &recibo, SIZE_MSG,-2,0);
		if(recibo.type == Tipo_Vegetal_Cocinar){
			respuesta.type = Tipo_Vegetal;
			printf("\nSoy cocinero %i y estoy cocinando un plato vegetal \n", (*aux));
		}else if(recibo.type == Tipo_Carne_Cocinar){ //Hay un pedido de carne
				respuesta.type = Tipo_Carne;
				printf("\nSoy cocinero %i y estoy cocinando un plato carne\n", (*aux));
		}
		sleep(1);
		msgsnd(queue_cliente_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
	}		
}

void * entregarComida(void * arg){
	tMessage recibo, respuesta1,respuesta2;
	int queue_cliente_Id = abrirColaClientes();
	int queue_comidas_Id = abrirColaComidas();
	
	while(1){
		msgrcv(queue_cliente_Id, &recibo, SIZE_MSG,-2,0); 
		if(recibo.type == Tipo_Vegetal_Pedido){//Hay un pedido de vegetal
			printf("Soy el camarero y me pidieron un plato vegetal\n");
			msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Vegetal, 0);
			printf("Soy el camarero y tome un plato de vegetal\n");
			respuesta1.type = Tipo_Vegetal_Listo;
			respuesta2.type =Tipo_Vegetal_Cocinar;
		}
		if(recibo.type == Tipo_Carne_Pedido){ //Hay un pedido de carne
			printf("Soy el camarero y me pidieron un plato carne\n");
			msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Carne, 0);
			printf("Soy el camarero y tome un plato de carne\n");
			respuesta1.type = Tipo_Carne_Listo;
			respuesta2.type =Tipo_Carne_Cocinar;
		}
		msgsnd(queue_cliente_Id,&respuesta1,SIZE_MSG,IPC_NOWAIT);
		msgsnd(queue_comidas_Id,&respuesta2,SIZE_MSG,IPC_NOWAIT);
	}
}

void * limpiarMesa(void * arg){
	
	tMessage recibo, respuesta;

	int queue_cliente_Id = abrirColaClientes();
	
	while(1){
		msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Limpiador, 0); // espero que se levante un cliente para liimpiar la mesa
		printf("\nSoy mesero y limpio la mesa\n");
		respuesta.type = Tipo_Mesas; // Se habilita una mesa
		msgsnd(queue_cliente_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
	}	
}

int abrirPrimeraColaClientes(){
	key_t key = ftok(FILE_PATH, 1);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	msgctl(queueId, IPC_RMID, NULL);
	
	queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	return queueId;
}

int abrirPrimeraColaComidas(){
	key_t key = ftok(FILE_PATH, 2);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	msgctl(queueId, IPC_RMID, NULL);
	
	queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	return queueId;
}

	
int main(int argc, char* argv[]) {
	srand(time(NULL));
	tMessage pedido;
	int queue_cliente_Id,queue_comidas_Id;
	queue_cliente_Id = abrirPrimeraColaClientes();
	queue_comidas_Id = abrirPrimeraColaComidas();
	
	pthread_t cliente[N],cocinero[C],camarero,limpiador;
	
	int *aux;
	for(int i=0; i < C; i++){
		aux = malloc(sizeof(int));
		*aux = i;
		pthread_create(&cocinero[i], NULL, cocinar, aux);
	}
	
	for(int i=0; i < N; i++){
		aux = malloc(sizeof(int));
		*aux = i;
		pthread_create(&cliente[i], NULL, comer, aux);
	}
	
	pthread_create(&camarero, NULL, entregarComida, NULL);
	pthread_create(&limpiador, NULL, limpiarMesa, NULL);
	
	
	pedido.type = Tipo_Mesas; 
	for(int i = 0; i < M; i++){
		msgsnd(queue_cliente_Id, &pedido, SIZE_MSG, IPC_NOWAIT);
	}
	pedido.type = Tipo_Carne_Cocinar;
	for(int i = 0; i < 5; i++){
		msgsnd(queue_comidas_Id, &pedido, SIZE_MSG, IPC_NOWAIT);
	}
	pedido.type = Tipo_Vegetal_Cocinar;
	for(int i = 0; i < 5; i++){
		msgsnd(queue_comidas_Id, &pedido, SIZE_MSG, IPC_NOWAIT);
	}
	
	for(int i=0; i < C; i++){pthread_join(cocinero[i], NULL);}
	
	for(int i=0; i < N; i++){pthread_join(cliente[i], NULL);}
	
	

}
