#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
/* ------------------------------------------------------------------------ */
/*			E N T Ê T E S    P E R S O N N E L L E S						*/
/* ------------------------------------------------------------------------ */
#include "../include/lib_train.h"

/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */

#define SERVER_IP "127.0.0.1" // Adresse IP du serveur G2R
#define SERVER_PORT 8890 // Port du serveur G2R
#define TRAIN_ID 1 // Numéro du train
#define TRAME_ID 1
#define XWAY_HEXA_TRAIN 0x01 //train 1
#define demande_g2r 1 //premier message vers le G2R pour réservation de la ressource
#define restitution_g2r 3 //fin d'utilisation de la ressource du train vers g2R 
#define MAX_MESSAGE_LENGTH 700

/* ------------------------------------------------------------------------ */
/*		V A R I A  B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */
int index_actuel = 0;
float position = 0;
int liste_parcourt_1[50] = {1,2,3, 81,70,78,77,13,104,105,7,8,9,92};/*à remplir avec le parcourt */
// mutex pour protéger l'accès à la variable position
pthread_mutex_t mutex_position = PTHREAD_MUTEX_INITIALIZER;

int main() {
	

	//1. Connexion du train au G2R en TCP/IP
	int length_parcourt = 12;
	
	int sd_g2r = connect_to_server(SERVER_IP,SERVER_PORT);
	printf("[TRAIN 1]Bien connecté au serveur g2r\n");
	int id_train = 1;
	char message_recu_train[MAX_MESSAGE_LENGTH];
	thread_argument_train t_args;
    	t_args.sockfd = sd_g2r;	
    	t_args.trajet = liste_parcourt_1;
    	t_args.id_train = id_train;
    	printf("id train dans les args : %d\n",t_args.id_train);
	lancement_thread_ppx(&t_args);

    	close(sd_g2r);
    	//close(sd_api);
    	return 0;
}

