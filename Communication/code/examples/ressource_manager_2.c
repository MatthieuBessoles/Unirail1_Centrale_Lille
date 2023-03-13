/****************************************************/
/* Application : ressource_manager                           */
/* Date : 18/02/2023                                */
/* Version : 2                                      */
/* Compilation : gcc ressource_manager_2.c -o ressource_manager_2          */
/* Usage : ./ressource_manager_2 */
/****************************************************/
/* ------------------------------------------------------------------------ */
/*			E N T Ê T E S    S T A N D A R D S						*/
/* ------------------------------------------------------------------------ */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>

/* ------------------------------------------------------------------------ */
/*			E N T Ê T E S    P E R S O N N E L L E S						*/
/* ------------------------------------------------------------------------ */
#include "../include/lib_g2r.h"
/* ------------------------------------------------------------------------ */
		/* D É F I N I T I O N S  D E  T Y P E S */
/* ------------------------------------------------------------------------ */
typedef void * (* pf_t)(void *) ;
/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */

#define NUM_RESSOURCES 10
#define LOCALIP "127.0.0.1" //IP du PC gestionnaire de ressources
#define LOCALPORT 8890 //Port du processus gestionnaire de ressource
#define LOCALPORT_T3 8888 //Port du processus gestionnaire de ressource
#define REMOTEPORT_T1_T3 3000 //T1/2 sont gérés sur le même PC
#define REMOTEPORT_T2_T4 2000 //T3/4 sont gérés sur le même PC
#define REMOTEIP_T1_T2 "127.0.0.1" //A MODIFIER
#define REMOTEIP_T3_T4 "127.0.0.1" //A MODIFIER
#define allocation_train 2 //bonne allocation de la ressource
#define acknowledge_fin_train 4 //acknowledge de fin d'utilisation de la ressource 
#define NB_TRAINS 4
/* ------------------------------------------------------------------------ */
/*			M A C R O - F O N C T I O N S						*/
/* ------------------------------------------------------------------------ */
#define CHECKERROR(var,val,msg)  if (var ==val) {perror (msg);}

# define CHECK_T(status , msg)						\
	if (0 != ( status )) {						\
		fprintf (stderr , " pthread erreur : %s\n", msg) ; 	\
		exit ( EXIT_FAILURE ) ;					\
	}
	
# define CHECK(status , msg)						\
	if (-1 == ( status )) {						\
	perror (msg) ;							\
	exit( EXIT_FAILURE ) ;						\
	}



/* ------------------------------------------------------------------------ */
/*		V A R I A  B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */


// Déclaration des sémaphores pour les ressources
sem_t ressource_semaphores[NUM_RESSOURCES];
int message_size = 3; //Il y a 3 octets dans les messages du train vers le G2R

/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */
void * gestion_train(void *client_sock_ptr);


int main(int argc, char *argv[]) {
	pthread_t client_trains[NB_TRAINS];
	double * status;
		
    	//déclaration des sockets pour les 4 trains
    
    	int server_fd, new_sockfd_1,new_sockfd_2,new_sockfd_3,new_sockfd_4;
    
    	socklen_t clilen;
    	struct sockaddr_in serv_addr;
    	int n;

    	// Initialisation des sémaphores pour chaque ressource
    	for (int i = 0; i < NUM_RESSOURCES; i++) {
        	sem_init(&ressource_semaphores[i], 0, 1);
    	}	

    	// Création du socket
    	server_fd = socket(AF_INET, SOCK_STREAM, 0);
    	if (server_fd < 0) {
        	perror("Erreur lors de la création du socket");
        	exit(1);
    	}
    	

    	// Initialisation des valeurs de la structure sockaddr_in
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_addr.s_addr = INADDR_ANY;
    	serv_addr.sin_port = htons(LOCALPORT);
    	

    	// Binding : attachement du socket au port
    	if (bind(server_fd , (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        	perror("Erreur lors du binding");
        	exit(1);
    	}
    	

    	// Écoute pour les 4 trains
    	listen(server_fd, 4);
    	clilen = sizeof(serv_addr); //même taille peut importe le train
    	
    	new_sockfd_1 = accept(server_fd, (struct sockaddr *)&serv_addr, &clilen);
	printf("jaccepte la connexion au train 1 \n");
    	if (new_sockfd_1 < 0) {
        	perror("Erreur lors de l'acceptation de la connexion entrante");
    		}
    		
    	int *client_fd_ptr_1 = malloc(sizeof(int));
        *client_fd_ptr_1 = new_sockfd_1;
        
        new_sockfd_2 = accept(server_fd, (struct sockaddr *)&serv_addr, &clilen);
	printf("jaccepte la connexion au train \n");
    	if (new_sockfd_2 < 0) {
        	perror("Erreur lors de l'acceptation de la connexion entrante");
    		}
    		
    	int *client_fd_ptr_2 = malloc(sizeof(int));
        *client_fd_ptr_2 = new_sockfd_2;
    	
        
        new_sockfd_3 = accept(server_fd, (struct sockaddr *)&serv_addr, &clilen);
	printf("jaccepte la connexion au train \n");
    	if (new_sockfd_3 < 0) {
        	perror("Erreur lors de l'acceptation de la connexion entrante");
    		}
    		
    	int *client_fd_ptr_3 = malloc(sizeof(int));
        *client_fd_ptr_3 = new_sockfd_3;
        
     	new_sockfd_4 = accept(server_fd, (struct sockaddr *)&serv_addr, &clilen);
	printf("jaccepte la connexion au train \n");
    	if (new_sockfd_4 < 0) {
        	perror("Erreur lors de l'acceptation de la connexion entrante");
    		}
    		
    	int *client_fd_ptr_4 = malloc(sizeof(int));
        *client_fd_ptr_4 = new_sockfd_4;
        
        
        printf("je rentre de le while \n");
    	/* Création des 4 threads pour les trains*/
	CHECK_T( pthread_create (&client_trains[0], NULL , (pf_t)gestion_train , (void *)client_fd_ptr_1), " pthread_create ()"); //train 1
  
	CHECK_T( pthread_create (&client_trains[1], NULL , (pf_t)gestion_train , (void *)client_fd_ptr_2), " pthread_create ()"); //train 2
		/* Création des 4 threads pour les trains*/
	CHECK_T( pthread_create (&client_trains[2], NULL , (pf_t)gestion_train , (void *)client_fd_ptr_3), " pthread_create ()"); //train 3
	CHECK_T( pthread_create (&client_trains[3], NULL , (pf_t)gestion_train , (void *)client_fd_ptr_4), " pthread_create ()"); //train 4
		
	CHECK_T ( pthread_join (client_trains[0], (void **) &status )," pthread_join ()") ;
	CHECK_T ( pthread_join (client_trains[1], (void **) &status )," pthread_join ()") ;
	CHECK_T ( pthread_join (client_trains[2], (void **) &status )," pthread_join ()") ;
	CHECK_T ( pthread_join (client_trains[3], (void **) &status )," pthread_join ()") ;
			
	sleep(5);
        // Déverrouille la ressource en relâchant le sémaphore
        //sem_post(&resource_semaphores[resource_index]);
    
	printf("[G2R] Fermeture des connexions\n");
    	// Ferme les connexions
    	close(new_sockfd_1);
    	close(new_sockfd_2);
    	close(new_sockfd_3);
    	close(new_sockfd_4);
    	close(server_fd);


	// Ferme les sémaphores
	for (int i = 0; i < NUM_RESSOURCES; i++) {
    		sem_destroy(&ressource_semaphores[i]);
    
		}

return 0;
}
	
/* ------------------------------------------------------------------------ */
/*		T H R E A D S  D E S  C L I E N T S						*/
/* ------------------------------------------------------------------------ */

void * gestion_train(void *client_sock_ptr){
	//pointeur vers un entier représentant le descripteur de fichier de la socket cliente.
	
    	while(1){
    		int client_sock = *((int *)client_sock_ptr);
    		unsigned char message_train[message_size];
		printf("[G2R] gestion du train %d\n",client_sock);
    	// 1. Lecture de la demande du client
    		int n = read(client_sock, message_train, sizeof(message_train));
    		if (n < 0) {
        		perror("Erreur lors de la lecture de la demande du client");
        		close(client_sock);
        		return NULL;
    		}
    	
    		printf("[G2R]Message reçu : ");
    		afficher_trame(message_train,message_size);
    		// 2. Identification de la ressource demandée
    		int *requete = lect_req(message_train, message_size);
    		printf("[G2R]recupération de la requête int ok\n");
    		for (int i = 0; i<message_size;i++){
    			printf("%d\n",requete[i]);
    			}
    		//2.2 Récupération de la ressource et des services associés
    		int message_type = requete[0];
    		int train_demandeur_id = requete[1];
    		int *my_services;
    		int id_service_demande;
    		printf("Le train %d envoie un message de type %d \n",train_demandeur_id,message_type);
    		switch (message_type){
    			case 1:
    				id_service_demande = requete[2];
    				printf("Il s'agit d'une demande d'accès au service %d jugé critique par le train %d\n",id_service_demande,train_demandeur_id);
    				int my_ressource = service_to_ressource (id_service_demande);
    				printf("ma ressource est : %i\n",my_ressource);
    				my_services = ressource_2_list_services ( my_ressource);
    	
    				//printf("mes services sont : \n");
				//2.1 Controle à effectuer
    	
    				free(requete);
    				/*if (my_ressource != 1){
    					printf("Pas le bon type de message ! \n");
    					exit(EXIT_FAILURE);
    					}*/
    				//autres actions à mener
    				//3. Tentative d'accès à la ressource
    				printf("Tentative d'accès à la ressource %d\n",my_ressource);
    				sem_wait(&ressource_semaphores [my_ressource-1]); //on attend que la ressource se libère
    				printf("Allocation de la ressource  %d au train %d\n",my_ressource,2);
        			//4. La ressource est disponible, envoi des trames au train
        			unsigned char* message_vers_train= malloc(15 * sizeof(unsigned char) * 2);
        			//unsigned char message_vers_train[50];
        			creation_message_vers_train (message_vers_train,2,train_demandeur_id, my_services, id_service_demande,1, client_sock,my_ressource);
        			printf("Message créé et envoyé !\n");
        			free(message_vers_train);
        			break;
   			case 3: 
   				printf("Confirmation de la libération d'une ressource par le train %d\n",train_demandeur_id);
   				//1. identification de la ressource liberee
   				int id_ressource_liberee = requete[2];
    				printf("Il s'agit d'une attestation de réalisation des services appartenant à la ressource %d par le train %d\n", id_ressource_liberee, train_demandeur_id);
				//2. Libération de la ressource
				sem_post(&ressource_semaphores [id_ressource_liberee -1]);
				printf("libération de la ressource %d ok\n", id_ressource_liberee);
				//3. Creation et envoi du message de confirmation de libération de la ressource au train
				int *my_service_fake[] = {0};
				id_service_demande = 0;
				printf("passage des param ok \n");
				unsigned char rep_vers_train[20];//2 octets
				creation_message_vers_train (rep_vers_train,4,train_demandeur_id, my_services, id_service_demande,1, client_sock,id_ressource_liberee);
				printf("Envoi de l'ackowledge par le G2R vers le train %d ok\n", train_demandeur_id);
				break;
			default:
				printf("Pas le bon type de message ! \n");
    				// Demande non valide
        			close(client_sock);
    				// Fermeture de la connexion avec le client
    				break;
    			}

    	}
    	return NULL;
	
}


