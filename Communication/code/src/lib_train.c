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
    	printf("id_train : %d\n",g2r_message->num_services);
    	memcpy(&(g2r_message->speed), message_recu + sizeof(int) * 3, sizeof(float));
    	printf("speed : %f\n",g2r_message->speed);
    	memcpy(&(g2r_message->dist), message_recu + sizeof(int) * 3 + sizeof(float), sizeof(float));
    	printf("dist : %f\n",g2r_message->dist);
    	printf("ajout des services  en cours\n");
    	memcpy(&((g2r_message->services_ok)), message_recu + sizeof(int) * (3) + sizeof(float)*2, sizeof(int)*g2r_message->num_services);
    	printf("ajout des services  fait\n");
             
	
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

  	//3. temps prochaine demande
  	
	printf("[thread demande] OK\n");
}


/* ------------------------------------------------------------------------ */
/*		G E S T I O N    D U    T E M P S
/* ------------------------------------------------------------------------ */
// Fonction appelée lorsque le timer expire
void send_position(int signum) {
    // Code à exécuter lorsque le timer expire, dans ce cas l'envoi de la position
    printf("Envoi de la position actuelle...\n");
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
}


