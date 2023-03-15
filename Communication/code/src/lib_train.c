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
//à changer avec le chemin de la librairie
//#include "/home/boyer/Bureau/Cesar/julius/include/check.h"
//pour l'utilisation de CHECK_IF

#include "../include/lib_train.h"
#include "../include/lib_g2r.h"
/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */
#define DEBUG


#define Tk 1
#define Akd 3
#define Akb 4
#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur
#define SERVER_PORT 8080  // Port du serveur
 //stop//
 
 

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




char* creation_message_vers_g2r(int message_type, int id_train, float pos, float speed,int id_serv_1, int id_serv_2, int id_serv_3,int sock_fd){
	printf("Création d'un message de type %d par le train d'id %d \n", message_type, id_train);
	// Allouer de la mémoire pour la trame
	TrainMessage_reception train_message_envoi;
    	int frame_length = sizeof(int) * 3 + sizeof(float) * 2;
    	char* frame = (char*) malloc(frame_length);
	
	
	train_message_envoi.message_type = message_type;
    	train_message_envoi.id_train = id_train;
    	train_message_envoi.position = pos;
    	train_message_envoi.speed = speed;
    	train_message_envoi.id_service_1 = id_serv_1;
    	train_message_envoi.id_service_2 = id_serv_2;
    	train_message_envoi.id_service_3 = id_serv_3;
    	// Copier les données de train_message dans la trame
    	memcpy(frame, &(train_message_envoi.message_type), sizeof(int));
    	memcpy(frame + sizeof(int), &(train_message_envoi.id_train), sizeof(int));
    	memcpy(frame + sizeof(int) * 2, &(train_message_envoi.position), sizeof(float));
    	memcpy(frame + sizeof(int) * 2 + sizeof(float), &(train_message_envoi.speed), sizeof(float));
    	memcpy(frame + sizeof(int) * 2 + sizeof(float)*2, &(train_message_envoi.id_service_1), sizeof(int));
    	memcpy(frame + sizeof(int) * 3 + sizeof(float)*2, &(train_message_envoi.id_service_2), sizeof(int));
    	memcpy(frame + sizeof(int) * 4 + sizeof(float)*2, &(train_message_envoi.id_service_3), sizeof(int));
	//write(sock_fd, frame, frame_length);
    	return frame;
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



void lect_req_g2r(char* message_recu,G2RMessage_reception* g2r_message){
	//cette fonction lit la requête reçue et en extrait les informations utilisées en fonction du type de message, puis renvoie la réponse sous forme de structure avec les informations rentrées
	printf("Les éléments reçus sont : \n");
	memcpy(&(g2r_message->message_type), message_recu, sizeof(int));
        printf("message_type : %d\n",g2r_message->message_type);
        memcpy(&(g2r_message->id_train), message_recu + sizeof(int), sizeof(int));
    	memcpy(&(g2r_message->num_services), message_recu + sizeof(int) * 2, sizeof(int));
    	memcpy(&(g2r_message->speed), message_recu + sizeof(int) * 3, sizeof(float));
    	memcpy(&(g2r_message->dist), message_recu + sizeof(int) * 3 + sizeof(float), sizeof(float));
    	memcpy(&(g2r_message->services_ok), message_recu + sizeof(int) * 3 + sizeof(float)*2, sizeof(int)*g2r_message->num_services);
        
        printf("service_type : %d\n",g2r_message->message_type);
        printf("id_train : %d\n",g2r_message->id_train);
        printf("id_train : %d\n",g2r_message->num_services);
        printf("speed : %f\n",g2r_message->speed);
        printf("dist : %f\n",g2r_message->dist);
	
		}

