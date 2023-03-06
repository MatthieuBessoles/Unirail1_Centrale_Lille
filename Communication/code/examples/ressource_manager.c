#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NB_RESSOURCES 10
#define vd(i) ((i)==NB_PHILOS -1)?0:(i)+1
#define vg(i) ((i)==0)?NB_PHILOS-1:(i)-1
#define RANDOM(min,max) (min)+rand()%((max)-(min)+1)

#define CHECK_T(status , msg)						\
	if (0 != ( status )) {						\
		fprintf (stderr , " pthread erreur : %s\n", msg) ; 	\
		exit ( EXIT_FAILURE ) ;					\
	}
	


typedef enum {P,S,M} etat_t;
etat_t e[NB_PHILOS] = {P};

typedef void * (* pf_t)(void *) ;


sem_t RESSOURCE[NB_RESSOURCES]; // création des 10 ressources définies par le RDPi
sem_t SP[NB_PHILOS];
void * philo(long no);
void majEtat (long no,etat_t new);
void afficherEtats();
void T(long i);

int main() {
 	pthread_t philos[NB_PHILOS];
 	long i;
 	//initialisation des 10 sémaphores " ressources " à 1
 	for(i = 0; i < NB_RESSOURCES; i++){
 		CHECK_T(sem_init(&RESSOURCE[i], 1, 0), "Problème initialisation sémaphore ressouce");
 		CHECK_T(pthread_create(&philos[i], NULL, (pf_t) philo, (void*) i ),"Problème création thread");
 	}
 	CHECK_T(sem_init(&mutex,1,0),"Problème initialisation sémaphore");

 	for(i = 0; i < NB_PHILOS; i++){
 		CHECK_T(sem_init(&SP[i], 1, 0), "Problème initialisation sémaphore");
 		CHECK_T(pthread_create(&philos[i], NULL, (pf_t) philo, (void*) i ),"Problème création thread");
 	}
 	afficherEtats();
 	CHECK_T(sem_post(&mutex), "Problème initialisation mutex"); //Top départ

 	//boucle pthread_join
 	for (i = 0 ; i < NB_PHILOS ; i++) {
		CHECK_T(pthread_join(philos[i], (void *) i ),"pthread_join ()") ;
		}
 	sem_destroy(&mutex);
 //boucle destroy SP
 	for (i = 0 ; i < NB_PHILOS ; i++) {
		CHECK_T(sem_destroy(&SP[i])," destroy") ;
		}
 	return 0;
}

void afficherEtats(){
 for(int i = 0; i < NB_PHILOS; i++){
 	switch(e[i]){
 		case P: printf("P__");
 			break;
 		case S: printf("S__");
 			break;
 		case M: printf("M__");
 			break;
 		}
 	}
 printf("\n");
}


void T (long i){
	if(e[i]==S && e[vd(i)] != M && e[vg(i)] != M){
		majEtat(i,M);
		sem_post(&SP[i]);
		}
	}

void majEtat(long no, etat_t new){
	e[no] = new;
	afficherEtats();
}

void * philo (long no){
	while(1){
		//Penser
		usleep(RANDOM(500,100)*1000);
		sem_wait(&mutex);
			majEtat(no,S);
			T(no);
		sem_post(&mutex);
		sem_wait(&SP[no]);
		//Manger
		usleep(RANDOM(50,100));
		sem_wait(&mutex);
			majEtat(no,P);
			T(vg(no));
			T(vd(no));
		sem_post(&mutex);
		}
	}
