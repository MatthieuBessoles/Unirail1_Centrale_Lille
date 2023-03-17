#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
//à changer avec le chemin de la librairie
//#include "/home/boyer/Bureau/Cesar/julius/include/check.h"
//pour l'utilisation de CHECK_IF

#include "../include/lib_train.h"
#include "../include/lib_g2r.h"
/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */
#define DEBUG
#define MAX_MESSAGE_LENGTH 500


#define Tk 1
#define Akd 3
#define Akb 4
#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur
#define SERVER_PORT 8080  // Port du serveur
 //stop//
 
double * status;
extern int index_actuel;
extern float position; 
extern int liste_parcourt_1[50]; /*à remplir avec le parcourt */
// mutex pour protéger l'accès à la variable position
extern pthread_mutex_t mutex_position; 
int convert_id_to_dico(int id, int type){
	int dico;
	switch (type){
		case Tk:
			dico = id;
			break;
		case Akd:
			dico = id+60;
			break;
		
		case Akb:
			dico = id+90;
			break;
			}
	return dico;
}



int mot_ecriture(unsigned char id_train){
	int mot;
	switch (id_train){
		case 0x01:
			mot = 1;
			break;
		case 0x02:
			mot = 4;
			break;
		case 0x03:
			mot = 7;
			break;
		case 0x04:
			mot = 10;
			break;
		}
	return (mot);
}

/* ------------------------------------------------------------------------ */
/*			C R E A I O N  D E S  M E S S A G E S 			*/
/* ------------------------------------------------------------------------ */		




void creation_message_vers_g2r(int message_type, int id_train, float pos, float speed,int id_serv_1, int id_serv_2, int id_serv_3,int sock_fd, int id_ressource){
	printf("Création d'un message de type %d par le train d'id %d \n", message_type, id_train);
	// Allouer de la mémoire pour la trame
	TrainMessage_envoi train_message_envoi;
    	int frame_length = sizeof(int) * 6 + sizeof(float) * 2;
    	char* frame = (char*) malloc(frame_length);
	
	
	train_message_envoi.message_type = message_type;
    	train_message_envoi.id_train = id_train;
    	train_message_envoi.position = pos;
    	train_message_envoi.speed = speed;
    	train_message_envoi.id_service_1 = id_serv_1;
    	train_message_envoi.id_service_2 = id_serv_2;
    	train_message_envoi.id_service_3 = id_serv_3;
    	train_message_envoi.id_ressource = id_ressource;
    	// Copier les données de train_message dans la trame
    	memcpy(frame, &(train_message_envoi.message_type), sizeof(int));
    	memcpy(frame + sizeof(int), &(train_message_envoi.id_train), sizeof(int));
    	memcpy(frame + sizeof(int) * 2, &(train_message_envoi.position), sizeof(float));
    	memcpy(frame + sizeof(int) * 2 + sizeof(float), &(train_message_envoi.speed), sizeof(float));
    	memcpy(frame + sizeof(int) * 2 + sizeof(float)*2, &(train_message_envoi.id_service_1), sizeof(int));
    	memcpy(frame + sizeof(int) * 3 + sizeof(float)*2, &(train_message_envoi.id_service_2), sizeof(int));
    	memcpy(frame + sizeof(int) * 4 + sizeof(float)*2, &(train_message_envoi.id_service_3), sizeof(int));
    	memcpy(frame + sizeof(int) * 5 + sizeof(float)*2, &(train_message_envoi.id_ressource), sizeof(int));
	write(sock_fd, frame, frame_length);
    	free(frame);
}
	





/* ------------------------------------------------------------------------ */
/*		CONNEXIONS TCP/IP et XWAY					*/
/* ------------------------------------------------------------------------ */	

int connect_to_server(char *remoteip, int remoteport) {
    int sock;
    struct sockaddr_in server;
    
    // Création de la socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Erreur: création de socket a échoué");
        return -1;
    }

    // Configuration de l'adresse du serveur
    server.sin_addr.s_addr = inet_addr(remoteip);
    server.sin_family = AF_INET;
    server.sin_port = htons(remoteport);

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erreur: connexion au serveur a échoué");
        return -1;
    }
    
    printf("Connecté au serveur G2R\n");
    
    return sock;
}	


void close_socket(int sock) {
    // Fermeture de la socket
    if (close(sock) == -1) {
        perror("Erreur lors de la fermeture de la socket");
        exit(EXIT_FAILURE);
    }
}



void lect_req_g2r(char* message_recu, G2RMessage_reception* g2r_message, int sock_fd){
	//cette fonction lit la requête reçue et en extrait les informations utilisées en fonction du type de message, puis renvoie la réponse sous forme de structure avec les informations rentrées
	printf("Les éléments reçus sont : \n");
	
	memcpy(&(g2r_message->message_type), message_recu, sizeof(int));
        printf("message_type : %d\n",g2r_message->message_type);
        memcpy(&(g2r_message->id_train), message_recu + sizeof(int), sizeof(int));
        printf("id_train : %d\n",g2r_message->id_train);
    	memcpy(&(g2r_message->num_services), message_recu + sizeof(int) * 2, sizeof(int));
    	printf("num_services : %d\n",g2r_message->num_services);
    	memcpy(&(g2r_message->speed), message_recu + sizeof(int) * 3, sizeof(float));
    	printf("speed : %f\n",g2r_message->speed);
    	memcpy(&(g2r_message->dist), message_recu + sizeof(int) * 3 + sizeof(float), sizeof(float));
    	printf("dist : %f\n",g2r_message->dist);
    	memcpy(&(g2r_message->id_ressource), message_recu + sizeof(int) * 3 + sizeof(float)*2, sizeof(int));
    	printf("id_ressource : %d\n",g2r_message->id_ressource);
    	printf("ajout des services  en cours\n");
    	g2r_message->services_ok = malloc(g2r_message->num_services * sizeof(int));
    	printf("Service %d: %d\n", 0, g2r_message->services_ok[0]);
    	for (int i = 0; i < g2r_message->num_services; i++) {
    		printf("Service %d: %d\n", i, g2r_message->services_ok[i]);
    		memcpy(&(g2r_message->services_ok[i]), message_recu + sizeof(int) * (4+i) + sizeof(float)*2, sizeof(int));
        	
        	}
        	
    	printf("ajout des services  fait\n");
             
	
}

/* ------------------------------------------------------------------------ */
/*		P R O C H A I N     M O U V E M E N T
/* ------------------------------------------------------------------------ */

int vitesse_to_service(int id_service){
	printf("on associe une vitesse au service %d\n",id_service);
	int vitesse;
	if (id_service== 1 || id_service == 2 || id_service ==6 || id_service == 8 || id_service == 10 || id_service ==18 || id_service == 14 || id_service == 17){
		vitesse = VITESSE_MAX;
		}
	else if (id_service <= 30){vitesse = VITESSE_VIRAGE;}
	else {
		vitesse = VITESSE_AIGUILLAGE;
		}
	printf("La vitesse %d a été associée au service %d\n",vitesse,id_service);
	return vitesse;
	}

bool contains(int arr[], int n, int x) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == x) {
            return true;
        }
    }
    return false;
}

int* prochain_mouvement(G2RMessage_reception* g2r_message){
	//l'objectif est de déterminer le prochain mouvement réalisé par le train en fonction des allocations du G2R et de son parcours
	printf("calcul du mouvement\n");
	//1. On détermine l'autorisation de mouvement
	int* mouvement = (int*)malloc(30);
	int service_act;
	int num_services = g2r_message->num_services;
	printf("[prochain mvt] num services = %d\n",num_services);
	int *services = g2r_message->services_ok;
	//printf("[prochain mvt]services ok = %d\n", services[0]);
	printf("[prochain mvt]services ok = %d\n", g2r_message->services_ok[0]);
	printf("[prochain mvt]index actuel =%d\n",index_actuel);
	int nb_mvts = 0; 
	int j = 0;
	int k=0 ;
	int taille = sizeof(liste_parcourt_1)/sizeof(int);
	int is_in_trajet = 1;
	while(is_in_trajet == 1 && (k+index_actuel < taille)){
        	if (contains(services,num_services,liste_parcourt_1 [k+index_actuel] )) {
            		mouvement[j+1] = liste_parcourt_1 [k+index_actuel];
			printf("[prochain mvt]mouvement %d =%d\n",j,mouvement[j+1]);
			nb_mvts+= 1;
			j+=1;
			k+=1;}
            	else {is_in_trajet = 0;}
            				}
	mouvement[0] = nb_mvts;
	printf("[prochain mvt]nb mouvement = %d\n", nb_mvts);
	//2. On associe les vitesses autorisées
	int mouvement_size = sizeof(mouvement)/sizeof(int);
	int vitesse_autor;
	int mouv;
	printf("[prochain mvt]size ok: %d\n", mouvement_size);
	for (int i=0; i<mouvement_size;i++){
		mouv=mouvement[i];
		printf("[prochain mvt] mouv = %d\n", mouv);
		vitesse_autor = vitesse_to_service(mouv);
		printf("[prochain mvt]vitesse autor = %d\n",vitesse_autor);
		printf("[prochain mvt]realloc size ok\n");
		mouvement[i+mouvement_size] = vitesse_autor;
		printf("[prochain mvt]vitesse1 ok\n");
		}
	printf("[prochain mvt]vitess add ok\n");
	return(mouvement);
	//renvoie une liste de services et leurs vitesses associées
	}
/* ------------------------------------------------------------------------ */
/*		T H R E A D      A V A N C E
/* ------------------------------------------------------------------------ */

void avancer(G2RMessage_reception* g2r_message){
	while (1){
		int i = 0;
		//1. calcule du prochain mouvement
		int *mouvement = prochain_mouvement(g2r_message); 
		int nb_mvts = mouvement[0];
		printf("[AVANCER] nb_mvt = %d\n",nb_mvts);
		int mouvement_size = 2*nb_mvts+1;
		printf("Dans le thread avancer\n");
		
		int vitesse;
		//2. Avancer en conséquence
		while (i < nb_mvts) {
		
			pthread_mutex_lock(&mutex_position);
        		// Calculer la distance parcourue
        		vitesse = mouvement[index_actuel+(mouvement_size-1)/2];
        		printf("[AVANCER] vitesse = %d\n",vitesse);
        		//1. On reserve l'acces à la mutex position
    		
        		position += T_e*vitesse;
        		//printf("distance_parcourue = %f\n",position);
        		// Actualiser l'index si la distance parcourue dépasse 150 cm
        		if (position >= LONGUEUR) {
            			index_actuel++;
            			position = 0.0;
        		}
        		i++;
        		pthread_mutex_unlock(&mutex_position);
        		// Attendre un temps pour simuler le déplacement du train
        		sleep(1);
        		
    		}
    		free(mouvement);
    	}
    
    
    }
		
	

/* ------------------------------------------------------------------------ */
/*		C R E A T I O N  D E S  D E U X  T H R E A D S 
		           P R I N C I P A U X
/* ------------------------------------------------------------------------ */

void lancement_thread_ppx(thread_argument_train* args){
	printf("Lancement du thread d'envoi de position\n");
	pthread_t thread_pos;//calcul et envoi de position
	printf("Lancement du thread de demande d'autorisation de mouvement\n");
	pthread_t thread_autor;//demande d'autorisation de mouvements
	CHECK_T( pthread_create (&thread_pos, NULL , (pf_t)gestion_position , args), " pthread_create ()");
	CHECK_T( pthread_create (&thread_autor, NULL , (pf_t)gestion_demande , args), " pthread_create ()");
	CHECK_T ( pthread_join (thread_pos, (void **) &status )," pthread_join ()") ; 
	CHECK_T ( pthread_join (thread_autor, (void **) &status )," pthread_join ()") ; 
	}
	
void gestion_position(void* args){
	int sockfd = (int) args; // conversion explicite de void* à int
  // utilisation de sockfd ici
  	// Initialisez le timer
    	init_timer();
	printf("[thread position] OK\n");
}


void gestion_demande(void* args){
  	//1. Première demande: les trois premiers services de mon trajet
  	thread_argument_train *t_args = (thread_argument_train*)args;
    	int sockfd = t_args->sockfd;
    	int *trajet = t_args->trajet;
    	int train_id = t_args->id_train ;
    	printf("gestion demande train %d\n",train_id);
    	int id_service_1 = trajet[0];
    	int id_service_2 = trajet[1];
    	int id_service_3 = trajet[2];
    	creation_message_vers_g2r(2, train_id, 0.2, 0.2,id_service_1, id_service_2,id_service_3,sockfd, 0);
    	printf("Envoi de la 1ere demande d'autorisation de mvt OK\n");
    	
    	
  	//2. attente de la bonne réception
  	G2RMessage_reception train_message;
  	char message_recu_train[MAX_MESSAGE_LENGTH];
	int n = read(sockfd,message_recu_train, MAX_MESSAGE_LENGTH);
	lect_req_g2r(message_recu_train, &train_message,sockfd);
	
	//3. Création du thread avance
	G2RMessage_reception *train_message_ptr = &train_message;
	printf("Lancement du thread avancer\n");
	pthread_t thread_avance;//calcul et envoi de position
	CHECK_T( pthread_create (&thread_avance, NULL , (pf_t)avancer , (void *)train_message_ptr), " pthread_create ()");
	CHECK_T ( pthread_join (thread_avance, (void **) &status )," pthread_join ()") ; 
  	//3. temps prochaine demande
  	sleep(2);
  	
  	
	printf("[thread demande] OK\n");
}


/* ------------------------------------------------------------------------ */
/*		G E S T I O N    D U    T E M P S
/* ------------------------------------------------------------------------ */
// Fonction appelée lorsque le timer expire
void send_position(int signum) {
    // Code à exécuter lorsque le timer expire, dans ce cas l'envoi de la position
    	//1. On reserve l'acces à la mutex position
    	pthread_mutex_lock(&mutex_position);
    	//2. On envoie le message au g2r
    	int message_type = 1;
    	//creation_message_vers_g2r(message_type, int id_train, float pos, float speed,int id_serv_1, int id_serv_2, int id_serv_3,int sock_fd, int id_ressource);
    	printf("Envoi de la position actuelle : %f\n",position);
    	pthread_mutex_unlock(&mutex_position);
}

// Fonction pour initialiser le timer
void init_timer() {
    struct sigaction sa;
    struct itimerval timer;

    // Spécifiez l'action à prendre lorsque le timer expire
    sa.sa_handler = &send_position;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    // Configurez le timer pour expirer toutes les 0.5 secondes
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 500000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 500000;

    // Démarrez le timer
    setitimer(ITIMER_REAL, &timer, NULL);
    while(1){
    	sleep(1);
    	}
}


