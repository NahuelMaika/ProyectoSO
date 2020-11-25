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

#define M 30	//Mesas
#define N 50	//Clientes
#define C 3

void * comer(void * z){ 
	int * aux = (int*)z;
	tMessage recibo, respuesta;

	key_t key = ftok(FILE_PATH, 2);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}

	msgrcv(queueId, &recibo, SIZE_MSG, TYPE_1, 0); // espero por mesa disponible
	printf("\nSoy el Cliente nro: %i y me sente en una mesa\n", (*aux)+1);
	
	if(rand()%2){
		strcpy(respuesta.body, "Vegetariano");
	}else{
		strcpy(respuesta.body, "Carnivoro");
	}
	
	respuesta.type = TYPE_2;
	msgsnd(queueId, &respuesta, SIZE_MSG, IPC_NOWAIT);
	printf("Soy cliente nro %i y pedi un plato %s\n", (*aux)+1, respuesta.body);
	
	msgrcv(queueId, &recibo, SIZE_MSG, TYPE_3, 0); // espero por mi pedido 
	
	printf("Soy cliente nro %i me dieron mi plato %s y voy a comer\n", (*aux)+1, recibo.body); // primero el camarero le tiene que entregar el plato?
	printf("Soy cliente nro %i y me voy\n", (*aux)+1);
	
	respuesta.type = TYPE_4;
	msgsnd(queueId, &respuesta, SIZE_MSG, IPC_NOWAIT);
	
	pthread_exit(0);
}

void * cocinar(void * z){
	int * aux = (int*)z;

	tMessage recibo, respuesta;

	key_t key = ftok(FILE_PATH, 2);
	if (key < 0) {
		report_and_exit("ftok");
	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {
		report_and_exit("msgget");
	}
	
	// resto un lugar en el semaforo cola = pongo un nuevo plato de comida
	while(1){
		msgrcv(queueId, &recibo, SIZE_MSG, TYPE_5, 0); // espero por pedido de plato carnivoro o vegetariano
		
		printf("\nSoy cocinero %i y estoy cocinando.......entrego un plato de comida\n", (*aux));
		sleep(1);
		
		respuesta.type = TYPE_6;
		strcpy(respuesta.body, recibo.body);
		msgsnd(queueId, &respuesta, SIZE_MSG, IPC_NOWAIT);
	}		
	pthread_exit(0);
}

/*
 * El camarero toma la comida de la cola y la entrega a un cliente que ha pedido una comida
 */
void * entrgarComida(void * arg){
	
	tMessage recibo, respuesta;

	key_t key = ftok(FILE_PATH, 2);
	if (key < 0) {	report_and_exit("ftok");	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {	report_and_exit("msgget");	}
	
	while(1){
		msgrcv(queueId, &recibo, SIZE_MSG, TYPE_2, 0); // espero por pedido de plato carnivoro o vegetariano

		printf("Soy camarero y ordene al chef hacer un plato\n");
		respuesta.type = TYPE_5; // Se ordena al chef que haga un plato
		strcpy(respuesta.body, recibo.body);
		msgsnd(queueId, &respuesta, SIZE_MSG, IPC_NOWAIT);
		
		
		msgrcv(queueId, &recibo, SIZE_MSG, TYPE_6, 0); // espero por pedido de plato carnivoro o vegetariano
		printf("\nSoy camarero recogi el plato del cliente y lo entrego\n");
		
		respuesta.type = TYPE_3; // Se entrega el plato----------------------
		strcpy(respuesta.body, recibo.body);
		msgsnd(queueId, &respuesta, SIZE_MSG, IPC_NOWAIT);
	}
	pthread_exit(0);
}

void * limpiarMesa(void * arg){
	
	tMessage recibo, respuesta;

	key_t key = ftok(FILE_PATH, 2);//89
	if (key < 0) {	report_and_exit("ftok");	}
	int queueId = msgget(key, 0666 | IPC_CREAT);
	if (queueId < 0) {	report_and_exit("msgget");	}
	
	while(1){
		msgrcv(queueId, &recibo, SIZE_MSG, TYPE_4, 0); // espero que se levante un cliente para liimpiar la mesa
		printf("\nSoy mesero y limpio la mesa\n");
		respuesta.type = TYPE_1; // Se habilita una mesa
		msgsnd(queueId, &respuesta, SIZE_MSG, IPC_NOWAIT);
	}	
	pthread_exit(0);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	tMessage pedido;
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

	pedido.type = TYPE_1; // hay mesa
	for(int i = 0; i < M; i++){
		//cargo 30 mesas vacias
		msgsnd(queueId, &pedido, SIZE_MSG, IPC_NOWAIT);
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
	
	pthread_create(&camarero, NULL, *entrgarComida, NULL);
	pthread_create(&limpiador, NULL, *limpiarMesa, NULL);
	
	pthread_join(camarero, NULL);
	pthread_join(limpiador, NULL);
	
	for(int i=0; i < N; i++){pthread_join(cliente[i], NULL);}
	
	for(int i=0; i < C; i++){pthread_join(cocinero[i], NULL);}

}
