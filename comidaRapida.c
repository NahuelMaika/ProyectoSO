#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


#define C 3 // son 3 cocineros
#define N 50 // son 50 clientes

sem_t cola, mesas, pedidoParaCamarero, limpiar, comida, clienteSeLevanta, plato;


/*
 * Un cliente come su comida y se va
 */
void * comer(int i){ // Pasar por parametro el nuemro de cliente para imprimir
	
	sem_wait(&mesas);
	printf("\nSoy el Cliente nro: %i y me sente en una mesa\n", i+1);
	sem_post(&pedidoParaCamarero);
	sleep(1);
	sem_wait(&plato);
	printf("Soy cliente nro %i me dieron mi pedido y voy a comer\n", i+1); 
	printf("Soy cliente nro %i y me voy\n", i+1);
	sem_post(&clienteSeLevanta);
	pthread_exit(0);
}

/*
 * Los cocineros preparan la comida y la ponen en la cola de capacidad 10
 */
void * cocinar(int i){
	// resto un lugar en el semaforo cola = pongo un nuevo plato de comida
	while(1){
		sem_wait(&cola);
		printf("\nSoy cocinero %i y estoy cocinando.......entrego un plato de comida\n", i);
		sleep(1);
		sem_post(&comida);
	}		
	pthread_exit(0);
}

/*
 * El camarero toma la comida de la cola y la entrega a un cliente que ha pedido una comida
 */
void * entrgarComida(void * arg){
	while(1){
		sem_wait(&pedidoParaCamarero);
		
		printf("\nSoy camarero me pidieron un plato, retiro y entrego comida\n");
		sem_wait(&comida);
		sem_post(&cola);
		sem_post(&plato);
	}
	pthread_exit(0);
}

/*
 * Un limpiador limpia la mesa cuando un cliente se va
 */
void * limpiarMesa(void * arg){
	while(1){
		sem_wait(&clienteSeLevanta);
		printf("\nSoy mesero y limpio la mesa\n");
		sem_post(&mesas);
	}	
	pthread_exit(0);
}

int main(){
	
	sem_init(&cola, 0, 10);
	sem_init(&mesas, 0, 30); 
	sem_init(&clienteSeLevanta, 0, 0);
	sem_init(&pedidoParaCamarero, 0, 0);
	sem_init(&limpiar, 0, 0);
	sem_init(&comida, 0, 0);
	sem_init(&plato, 0, 0);

	pthread_t cliente[N], cocinero[C], camarero, limpiador;
	for(int i=0; i < C; i++){
		pthread_create(&cocinero[i], NULL, *cocinar, i);
	}
	
	for(int i=0; i < N; i++){
		pthread_create(&cliente[i], NULL, *comer, i);
	}
	
	pthread_create(&camarero, NULL, *entrgarComida, NULL);
	pthread_create(&limpiador, NULL, *limpiarMesa, NULL);
	
	pthread_join(camarero, NULL);
	pthread_join(limpiador, NULL);
	
	for(int i=0; i < N; i++){pthread_join(cliente[i], NULL);}
	
	for(int i=0; i < C; i++){pthread_join(cocinero[i], NULL);}
	
	return 0;
}
