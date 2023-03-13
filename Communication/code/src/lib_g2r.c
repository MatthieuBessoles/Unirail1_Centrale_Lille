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

/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */
//pour l'utilisation de CHECK_IF
#define DEBUG


/* ------------------------------------------------------------------------ */
/*		V A R I A  B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */
int NB_MAX_SERVICES = 6;
int NUM_RESSOURCES = 10;

int R_1 [] = {32,42}; // ensemble des id des services qui appartiennent à la ressource 1
int length_1 = sizeof(R_1) / sizeof(R_1[0]);
int R_2 [] = {33,43}; // ensemble des id des services qui appartiennent à la ressource 2
int length_2 = sizeof(R_2) / sizeof(R_2[0]);
int R_3 [] = {29}; // ensemble des id des services qui appartiennent à la ressource 3
int length_3 = sizeof(R_3) / sizeof(R_3[0]);
int R_4 [] = {53,63,11,31}; // ensemble des id des services qui appartiennent à la ressource 4
int length_4 = sizeof(R_4) / sizeof(R_4[0]);
int R_5 [] = {32,12,9,13,33}; // ensemble des id des services qui appartiennent à la ressource 5
int length_5 = sizeof(R_5) / sizeof(R_5[0]);
int R_6 [] = {7,37}; // ensemble des id des services qui appartiennent à la ressource 6
int length_6 = sizeof(R_6) / sizeof(R_6[0]);
int R_7 [] = {27}; // ensemble des id des services qui appartiennent à la ressource 7
int length_7 = sizeof(R_7) / sizeof(R_7[0]);
int R_8 [] = {28}; // ensemble des id des services qui appartiennent à la ressource 8
int length_8 = sizeof(R_8) / sizeof(R_8[0]);
int R_9 [] = {52,62}; // ensemble des id des services qui appartiennent à la ressource 9
int length_9 = sizeof(R_9) / sizeof(R_9[0]);
int R_10 [] = {50,60};// ensemble des id des services qui appartiennent à la ressource 10
int length_10 = sizeof(R_10) / sizeof(R_10[0]);



/* ------------------------------------------------------------------------ */
/*		CONNEXIONS TCP/IP et XWAY					*/
/* ------------------------------------------------------------------------ */	

int *lect_req(unsigned char* message_recu, int message_size){
	//cette fonction lit la requête reçue et en extrait les informations utilisées: type de message (req ou ack), id_service demandé et id_train qui le demande. L'id sera ensuite utilisé dans la fonction service_to_ressource
        printf("Le message recu contient %i octets\n",message_size);
        int id_service_demande;
        int id_train_demandeur;
        int message_type;
        
        int* liste_param_recus = malloc(message_size * sizeof(int));
        
        for (int i=0;i<message_size;i++){
        	
        	liste_param_recus[i] = hex_to_int((unsigned char)message_recu[i]);
		printf("Message_content : %d\n",liste_param_recus[i]);
		}
        return liste_param_recus;
	
}
int service_to_ressource(int id_service){
	int id_ressource;
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
         for (int i=0; i<length_10;i++){
		if (R_10[i] == id_service) {
            		id_ressource = 10;
           		break;
           		}
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
		case 10:
			list_services = R_10;
			break;
		default:
			printf("Problème avec la correspondance ressource -> services");
			exit(0);
		}

	return list_services;
	}

void generate_hex_message(int* int_list, int list_size) {
    unsigned char* message = malloc(list_size * sizeof(unsigned char) * 2);
    for (int i = 0; i < list_size; i++) {
        sprintf((char*)(message + (i * 2)), "%02X", int_list[i]);
    }
    printf("%s\n", message);
    free(message);
}

unsigned int int_to_hex(int num) {
  return (unsigned int)num;
}

int hex_to_int(unsigned char hex) {
  return (int)hex;
}

void afficher_trame(unsigned char* trame,int taille) {
	printf("la taille est de : %i\n",taille);
    	int i;
    	for (i = 0; i < taille; i++) {
        	printf("%04x ", trame[i]);
    	}
    	printf("\n");
}
int creation_message_vers_train(unsigned char * message,int message_type, int id_train, int* list_services, int id_service, int accepte, int sock_fd,int id_ressource){
	printf("[G2R]Création d'un message de type %d par le G2R vers le train d'id %d \n", message_type, id_train);
	int list_size = sizeof(list_services) / sizeof(int);
	printf("[G2R]il faut demander %i services \n",list_size);
	int NB_OCTETS;
	if (message_type == 2){
		NB_OCTETS = 3+list_size;}
	else
		{NB_OCTETS = 2;
	}
	printf("[G2R]nb d'octets = %i\n",NB_OCTETS);
	
	//unsigned char* message = (unsigned char*)malloc((list_size));  // Allocate memory for the XWAY message
	switch (message_type){
		case 2:	
			//allocation de la ressource au train id_train, composée de plusieurs services
			
			message[0] = int_to_hex(message_type);
			printf("message_0 = %hx\n",message[0]);
			message[1] = int_to_hex(id_train);
			message[2] = int_to_hex(list_size);
			message[3] = int_to_hex(id_ressource);
			for (int i = 1; i <list_size+1; i++) {
				message[i+3] = int_to_hex(list_services[i]); 
    }				
			break;
		case 4:	//acknowledge fin d'utilisation de la ressource	
			printf("[G2R]message de type ackowledge\n");
			message[0] = int_to_hex(message_type);
			message[1] = int_to_hex(id_train);	
			break;
		default:
			
			printf("[G2R]Pas le bon type de message");
			exit(EXIT_FAILURE);
		}
	printf("[G2R]le message envoyé par le G2R vers le train est : ");
	afficher_trame(message,NB_OCTETS);
	size_t message_size = strlen((char*)message);
        printf("[G2R]message de taille %ld\n",message_size);
        write(sock_fd, message, message_size);
   	printf("[G2R]Message envoyé au train %d\n",id_train);
   	return 0;
}
     

