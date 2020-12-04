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

#define M 30	//Mesas 30
#define N 50	//Clientes 50
#define C 3 	//Cocineros
#define K 10    //Espacio en la cola de comida

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

int abrirColaCamarero(){
	key_t key = ftok(FILE_PATH, 3);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	return queueId;
}

void * comer(void * z){ 
	int * aux = (int*)z;
	tMessage recibo, respuesta;

	int queue_cliente_Id, queue_camarero_Id;
	queue_cliente_Id=abrirColaClientes();
	queue_camarero_Id=abrirColaCamarero();

	msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Mesas, 0); // espero por mesa disponible
	printf("\nSoy el Cliente nro: %i y me sente en una mesa\n", (*aux)+1);
	
	if(rand()%2){
		respuesta.type = Tipo_Vegetal;
		msgsnd(queue_camarero_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
		printf("Soy cliente nro %i y pedi un plato vegetal\n", (*aux)+1);
		
		msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Vegetal, 0);//espero por mi pedido vegetal
		
		printf("Soy cliente nro %i me dieron mi plato vegetal y voy a comer\n", (*aux)+1);
		
	}else{
		respuesta.type = Tipo_Carne;
		msgsnd(queue_camarero_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
		printf("Soy cliente nro %i y pedi un plato carne\n", (*aux)+1);
		
		msgrcv(queue_cliente_Id, &recibo, SIZE_MSG, Tipo_Carne, 0);//espero por mi pedido carne
		
		printf("Soy cliente nro %i me dieron mi plato carne y voy a comer\n", (*aux)+1);
	}

	printf("Soy cliente nro %i y me voy\n", (*aux)+1);
	
	respuesta.type = Tipo_Limpiador;
	msgsnd(queue_cliente_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
	
	pthread_exit(0);
}

void * cocinar(void * z){
	int * aux = (int*)z;

	tMessage recibo, respuesta;
	int queue_comidas_Id = abrirColaComidas();
	
	while(1){
		msgrcv(queue_comidas_Id, &recibo, SIZE_MSG, Tipo_Cola_Comida, 0); // espero a que haya lugar en la cola para cocinar

		if(rand()%2){
			respuesta.type = Tipo_Vegetal;
			printf("\nSoy cocinero %i y estoy cocinando un plato vegetal \n", (*aux));
		}else{
			respuesta.type = Tipo_Carne;
			printf("\nSoy cocinero %i y estoy cocinando un plato carne\n", (*aux));
		}
		msgsnd(queue_comidas_Id, &respuesta, SIZE_MSG, IPC_NOWAIT);
	}		
	pthread_exit(0);
}

/*
 * El camarero toma la comida de la cola y la entrega a un cliente que ha pedido una comida
 */
void * entregarComida(void * arg){
	
	tMessage recibo, respuesta;
	
	int queue_cliente_Id,queue_comidas_Id,queue_camarero_Id;
	queue_cliente_Id =abrirColaClientes();
	queue_comidas_Id = abrirColaComidas();
	queue_camarero_Id = abrirColaCamarero();
	printf("SOY CAMARERO\n");
	while(1){
		
		msgrcv(queue_camarero_Id, &recibo, SIZE_MSG,0,0); //Hay un pedido de vegetal
		if(recibo.type == Tipo_Vegetal){
			printf("Soy el camarero y me pidieron un plato vegetal\n");
			msgrcv(queue_comidas_Id, &recibo, SIZE_MSG, Tipo_Vegetal, 0);
			printf("Soy el camarero y tome un plato de vegetal\n");
		}
		if(recibo.type == Tipo_Carne){ //Hay un pedido de carne
			printf("Soy el camarero y me pidieron un plato carne\n");
			msgrcv(queue_comidas_Id, &recibo, SIZE_MSG, Tipo_Carne, 0);
			printf("Soy el camarero y tome un plato de carne\n");
		}
		
		respuesta.type = recibo.type;
		msgsnd(queue_cliente_Id,&respuesta,SIZE_MSG,IPC_NOWAIT);
		respuesta.type =Tipo_Cola_Comida;
		msgsnd(queue_comidas_Id,&respuesta,SIZE_MSG,IPC_NOWAIT);
	}
	pthread_exit(0);
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
	pthread_exit(0);
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

int abrirPrimeraColaCamarero(){
	key_t key = ftok(FILE_PATH, 3);
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
	int queue_cliente_Id,queue_comidas_Id,queue_camarero_Id;
	queue_cliente_Id = abrirPrimeraColaClientes();
	queue_comidas_Id = abrirPrimeraColaComidas();
	queue_camarero_Id = abrirPrimeraColaCamarero();

	pedido.type = Tipo_Mesas; // hay mesa
	for(int i = 0; i < M; i++){
		//cargo 30 mesas vacias
		msgsnd(queue_cliente_Id, &pedido, SIZE_MSG, IPC_NOWAIT);
	}
	pedido.type = Tipo_Cola_Comida; // hay espacio en la cola de comida
	for(int i = 0; i < K; i++){
		//cargo 10 espacios vacios en la cola de comida
		msgsnd(queue_comidas_Id, &pedido, SIZE_MSG, IPC_NOWAIT);
	}

	pthread_t cliente[N], cocinero[C], camarero, limpiador;
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
	
	pthread_create(&camarero, NULL, *entregarComida, NULL);
	pthread_create(&limpiador, NULL, *limpiarMesa, NULL);
	
	pthread_join(camarero, NULL);
	pthread_join(limpiador, NULL);
	
	for(int i=0; i < N; i++){pthread_join(cliente[i], NULL);}
	
	for(int i=0; i < C; i++){pthread_join(cocinero[i], NULL);}

}
