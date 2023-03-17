#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <pthread.h>
//à changer avec le chemin de la librairie
#include "../include/lib_g2r.h"
#include "../include/lib_train.h"




/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */
//pour l'utilisation de CHECK_IF
#define DEBUG


/* ------------------------------------------------------------------------ */
/*		V A R I A B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */
double * status;
int NB_MAX_SERVICES = 12;

extern sem_t ressource_semaphores[NUM_RESSOURCES];
int message_size = 3; //Il y a 3 octets dans les messages du train vers le G2R

int R_1 [] = {71,91,70,90,78,108,76,106,72,102,25,26}; // ensemble des id des services qui appartiennent à la ressource 1
int length_1 = sizeof(R_1) / sizeof(R_1[0]);
int R_2 [] = {12}; // ensemble des id des services qui appartiennent à la ressource 2
int length_2 = sizeof(R_2) / sizeof(R_2[0]);
int R_3 [] = {5,13}; // ensemble des id des services qui appartiennent à la ressource 3
int length_3 = sizeof(R_3) / sizeof(R_3[0]);
int R_4 [] = {74,104,75,105,7}; // ensemble des id des services qui appartiennent à la ressource 4
int length_4 = sizeof(R_4) / sizeof(R_4[0]);
int R_5 [] = {8}; // ensemble des id des services qui appartiennent à la ressource 5
int length_5 = sizeof(R_5) / sizeof(R_5[0]);
int R_6 [] = {9}; // ensemble des id des services qui appartiennent à la ressource 6
int length_6 = sizeof(R_6) / sizeof(R_6[0]);
int R_7 [] = {62,92,63,93,64,94,65,95}; // ensemble des id des services qui appartiennent à la ressource 7
int length_7 = sizeof(R_7) / sizeof(R_7[0]);
int R_8 [] = {66,96}; // ensemble des id des services qui appartiennent à la ressource 8
int length_8 = sizeof(R_8) / sizeof(R_8[0]);
int R_9 [] = {11}; // ensemble des id des services qui appartiennent à la ressource 9
int length_9 = sizeof(R_9) / sizeof(R_9[0]);


/* ------------------------------------------------------------------------ */
/*		CONNEXIONS TCP/IP et XWAY					*/
/* ------------------------------------------------------------------------ */



void lect_req_train(char* message_recu, TrainMessage_reception* train_message,int sock_fd){
	//cette fonction lit la requête reçue et en extrait les informations utilisées en fonction du type de message, puis renvoie la réponse sous forme de structure avec les informations rentrées
	
	printf("Les éléments reçus sont : \n");
	memcpy(&(train_message->message_type), message_recu, sizeof(int));
        printf("message_type : %d\n",train_message->message_type);
        memcpy(&(train_message->id_train), message_recu + sizeof(int), sizeof(int));
    	memcpy(&(train_message->position), message_recu + sizeof(int) * 2, sizeof(float));
    	memcpy(&(train_message->speed), message_recu + sizeof(int) * 2 + sizeof(float), sizeof(float));
    	memcpy(&(train_message->id_service_1), message_recu + sizeof(int) * 2 + sizeof(float)*2, sizeof(int));
    	memcpy(&(train_message->id_service_2), message_recu + sizeof(int) * 3 + sizeof(float)*2, sizeof(int));
    	memcpy(&(train_message->id_service_3), message_recu + sizeof(int) * 4 + sizeof(float)*2, sizeof(int));
        train_message->sockfd = sock_fd;
        printf("service_type : %d\n",train_message->message_type);
        printf("id_train : %d\n",train_message->id_train);
        printf("position : %f\n",train_message->position);
        printf("speed : %f\n",train_message->speed);
        printf("id_service_1 : %d\n",train_message->id_service_1);
        printf("id_service_2 : %d\n",train_message->id_service_2);
        printf("id_service_3 : %d\n",train_message->id_service_3);
	printf("sock_fd : %d\n",train_message->sockfd);
		}
		
		
int service_to_ressource(int id_service){
	int id_ressource = 0;
	for (int i=0; i<length_1;i++){
		if (R_1[i] == id_service) {
            		id_ressource = 1;
           		break;
           		}
           	}
	for (int i=0; i<length_2;i++){
		if (R_2[i] == id_service) {
            		id_ressource = 2;
           		break;
           		}
           	}
        for (int i=0; i<length_3;i++){
		if (R_3[i] == id_service) {
            		id_ressource = 3;
           		break;
           		}
           	}
        for (int i=0; i<length_4;i++){
		if (R_4[i] == id_service) {
            		id_ressource = 4;
           		break;
           		}
           	}
        for (int i=0; i<length_5;i++){
		if (R_5[i] == id_service) {
            		id_ressource = 5;
           		break;
           		}
           	}
        for (int i=0; i<length_6;i++){
		if (R_6[i] == id_service) {
            		id_ressource = 6;
           		break;
           		}
           	}
        for (int i=0; i<length_7;i++){
		if (R_7[i] == id_service) {
            		id_ressource = 7;
           		break;
           		}
           	}
        for (int i=0; i<length_8;i++){
		if (R_8[i] == id_service) {
            		id_ressource = 8;
           		break;
           		}
           	}
        for (int i=0; i<length_9;i++){
		if (R_9[i] == id_service) {
            		id_ressource = 9;
           		break;
           		}
           	}
        
	if(id_ressource == 0){
		printf("Le service demandé n'appartient pas à une ressource critique \n");
		}
	return id_ressource;
	}

int * ressource_2_list_services(int id_ressource){
	int* list_services = malloc(NB_MAX_SERVICES* sizeof(int));
	switch (id_ressource){
		case 1:
			list_services = R_1;
			break;
		case 2:
			list_services = R_2;
			break;
		case 3:
			list_services = R_3;
			break;
		case 4:
			list_services = R_4;
			break;
		case 5:
			list_services = R_5;
			break;
		case 6:
			list_services = R_6;
			break;
		case 7:
			list_services = R_7;
			break;
		case 8:
			list_services = R_8;
			break;
		case 9:
			list_services = R_9;
			break;
		default:
			printf("Problème avec la correspondance ressource -> services");
			exit(0);
		}

	return list_services;
	}




/* ------------------------------------------------------------------------ */
/*			C R E A T I O N  D E S  M E S S A G E S 			*/
/* ------------------------------------------------------------------------ */		



void creation_message_vers_train(int message_type, int id_train, float dist, float speed,int num_services_ok, int* services,int id_zone_suivi, int id_serv_ok,int sock_fd,int id_ressource){
	printf("[G2R]Création d'un message de type %d par le G2R vers le train d'id %d \n", message_type, id_train);
	// Allouer de la mémoire pour la trame
	G2RMessage_envoi g2r_message_envoi;
    	int frame_length = sizeof(int) * (4+num_services_ok) + sizeof(float) * 2;
    	char* frame = (char*) malloc(frame_length);
	
	
	g2r_message_envoi.message_type = message_type;
    	g2r_message_envoi.id_train = id_train;
    	g2r_message_envoi.num_services = num_services_ok;
    	g2r_message_envoi.speed = speed;
    	g2r_message_envoi.dist =dist;
    	g2r_message_envoi.id_ressource =id_ressource;
    	g2r_message_envoi.services_ok = services;
    	// Copier les données de train_message dans la trame
    	memcpy(frame, &(g2r_message_envoi.message_type), sizeof(int));
    	memcpy(frame + sizeof(int), &(g2r_message_envoi.id_train), sizeof(int));
    	memcpy(frame + sizeof(int) * 2, &(g2r_message_envoi.num_services), sizeof(int));
    	memcpy(frame + sizeof(int) * 3, &(g2r_message_envoi.speed), sizeof(float));
    	memcpy(frame + sizeof(int) * 3 + sizeof(float), &(g2r_message_envoi.dist), sizeof(float));
    	memcpy(frame + sizeof(int) * 3 + sizeof(float)*2, &(g2r_message_envoi.id_ressource), sizeof(int));
    	for (int i = 0; i < num_services_ok; i++) {
    		memcpy(frame + sizeof(int) * (4+i) + sizeof(float)*2, &(g2r_message_envoi.services_ok[i]), sizeof(int));
    		printf("[creation_message_vers_train] service_ok %d = %d\n",i,g2r_message_envoi.services_ok[i]);}
	write(sock_fd, frame, frame_length);
	free(frame);
    	
}
 

// Fonction pour traiter un message
void handle_message_type(void* args) {
    // Traiter le message recu
    thread_argument_g2r *t_args = (thread_argument_g2r*)args;
    int sockfd = t_args->sockfd;
    int message_type = t_args->message_type;
    int train_id = t_args->id_train ;
    float speed = t_args->speed ;
    float pos = t_args->position ;
    int id_ressource = t_args->id_ressource;
    int id_service_1 = t_args->id_service_1;
    int id_service_2 = t_args->id_service_2;
    int id_service_3 = t_args->id_service_3;
    printf("[G2R] Gestion d'un message de type %d\n",message_type);
    
    switch (message_type){
    	case 1:
    		check_position(message_type,train_id,speed,pos,id_service_1, id_service_2,id_service_3,sockfd);
    		break;
    	case 2:
    		
    		printf("Gestion de la Demande d'autorisation de mouvement ();");
    		autorisation(message_type,train_id,speed,pos,id_service_1, id_service_2,id_service_3,sockfd,id_ressource);
    		break;
    	case 3:
    		printf("traitement 3");
    		break;
    	case 4:
    		printf("traitement 4");
    		break;
    	case 5:
    		printf("traitement 5");
    		break;
    		
    	}
}


void check_position(int message_type, int train_id, float pos, float speed, int id_service_1, int id_service_2, int id_service_3, int sockfd){
	//calcul de la vitesse autorisée
	printf("vitesse recu : %f\n",speed);
	float vitesse = speed;
	float vitesse_autor = vitesse/2;
	printf("La vitesse autorisee est : %f\n", vitesse_autor);
	int services[2] = {0,1};
	creation_message_vers_train(12, train_id, pos, vitesse_autor,2.0, services,0, 3,sockfd,0);
	}
	

	

void autorisation(int message_type, int train_id, float pos, float speed, int id_service_1, int id_service_2, int id_service_3, int sockfd, int id_ressource){
	//détermination de l'autorisation de mouvement
	
	int services_demandes[3] = {id_service_1,id_service_2, id_service_3};
	int is_ressource = 0;
	int i = 0;
	int *service_autor= NULL; //pointeur vers la liste de services autor dont on ne connait pas la taille initialement
	int service_demande;
	while (is_ressource ==0 && i<3){
		i = i+1;
		service_demande = services_demandes[i-1];
		service_autor = realloc(service_autor, i * sizeof(int)); // réallocation dynamique de mémoire pour la liste
		service_autor[i-1] = service_demande;
		is_ressource = service_to_ressource(service_demande);
		}
	switch (is_ressource){
		case 0:
			//dans le cas où ce n'est pas une ressource, on autorise tous les services, avec les vitesses correspondantes au service
			float vitesse_autor = 3.0;
			creation_message_vers_train(11, train_id,pos,vitesse_autor,3,service_autor,0,3,sockfd,is_ressource);//3 services autorisés
			break;
		case 1:
			
			//le service demandé appartient à la ressource critique 1		
			//1. Réservation de la ressource sous forme de sémaphore
			
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_1;j++){
				service_demande = R_1[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
		case 2:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_2;j++){
				service_demande = R_2[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
		case 3:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_3;j++){
				service_demande = R_3[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
		case 4:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_4;j++){
				service_demande = R_4[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
		case 5:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_5;j++){
				service_demande = R_5[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
		case 6:
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			//le service demandé appartient à la ressource critique 1
			for (int j = 0; j <length_6;j++){
				service_demande = R_6[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
			//1. On alloue les services de la ressource critique
			
		case 7:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_7;j++){
				service_demande = R_7[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
			//1. On alloue les services de la ressource critique
					
		case 8:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_8;j++){
				service_demande = R_8[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
			//1. On alloue les services de la ressource critique
					
		case 9:
			
			//le service demandé appartient à la ressource critique 1
			printf("Tentative d'accès à la ressource %d\n",is_ressource);
    			sem_wait(&ressource_semaphores [is_ressource-1]); //on attend que la ressource se libère
    			printf("Allocation de la ressource  %d au train %d\n",is_ressource,train_id);
			for (int j = 0; j <length_9;j++){
				service_demande = R_9[j];
				service_autor = realloc(service_autor, j * sizeof(int)); // réallocation dynamique de mémoire pour la liste
				service_autor[i+j] = service_demande;
				}
			break;
			//1. On alloue les services de la ressource critique
					
		default:
			printf("erreur service demande");
			break;
		}
	int num_services = sizeof(service_autor)/sizeof(int);
	creation_message_vers_train(11, train_id,  3.0,2.0,num_services, service_autor,1,1,sockfd,is_ressource);
	}		

// Thread pour gérer un train


/* ------------------------------------------------------------------------ */
/*		C R E A T I O N  D E S  T H R E A D S 
		D E  T R A I T E M E N T  D E S  M E S S A G E S			*/
/* ------------------------------------------------------------------------ */

void lancement_thread(TrainMessage_reception* train_message, int sock){
	
	pthread_t thread_id;
	printf("Lancement du thread de gestion de message\n");
	CHECK_T( pthread_create (&thread_id, NULL , (pf_t)handle_message_type , (void *)train_message), " pthread_create ()");
	CHECK_T ( pthread_join (thread_id, (void **) &status )," pthread_join ()") ; 
	}

