#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#define cant_hilos 2

sem_t semLeche,semComprando;
pthread_mutex_t mutex;

void * comportamiento_comp(void *data){
	int *aux;
	aux = (int *) data;
	int i = *aux;
	sleep(1);
	while(1){
		sleep(1);
		pthread_mutex_lock(&mutex);
		if(sem_trywait(&semLeche)){//No hay leche
			printf("(Compañero %i): Miro el refrigerador y no hay leche\n",i);
			if(sem_trywait(&semComprando)){//Si alguien fue a comprar(Si semComprando esta en 0)
					printf("(Compañero %i): Alguien ya fue a comprar leche\n",i);
					printf("(Compañero %i): Voy a hacer otras cosas\n",i);
					pthread_mutex_unlock(&mutex);
					sleep(1);
				}else{//No hay nadie comprando(Si semComprando esta en 1)
					pthread_mutex_unlock(&mutex);
					printf("(Compañero %i): Voy al supermercado\n",i);
					 sleep(1);
					printf("(Compañero %i): Llego al supermercado\n",i);
					printf("(Compañero %i): Compro leche\n",i);
					printf("(Compañero %i): Llego a la casa y guardo la leche\n",i);
					pthread_mutex_lock(&mutex);
					sem_post(&semLeche);
					sem_post(&semLeche);
					sem_post(&semLeche);
					sem_post(&semLeche);
					pthread_mutex_unlock(&mutex);
					sem_post(&semComprando);
					sleep(1);
				}
		}else{//Hay leche
			printf("(Compañero %i): Miro el refrigerador y hay leche\n",i);
			printf("(Compañero %i): Consumo una botella de leche\n",i);
			pthread_mutex_unlock(&mutex);
			sleep(1);		
		}
		sleep(1);
	}
}

void crearSemaforos(){
	sem_init(&semLeche,0,4);
	sem_init(&semComprando,0,1);
}

int main(){
	pthread_t hilos[cant_hilos];
	crearSemaforos();
	int *aux;
	
	
	for(int i = 0;i<cant_hilos;i++){
		aux=malloc(sizeof(int));
		*aux=i;
		pthread_create(&hilos[i],NULL,comportamiento_comp,aux);
	}
	
	for(int i = 0;i<cant_hilos;i++){
		pthread_join(hilos[i],NULL);
	}
	
	exit(0);
}
