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
/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */
#define DEBUG


#define Tk 1
#define Tik 2
#define Akd 3
#define Akb 4
#define Tjkd 5
#define Tjkb 6
#define PAkd 7
#define PAkb 8
#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur
#define SERVER_PORT 8080  // Port du serveur
 //stop//
 
 



unsigned int int_to_hex(int num) {
  return (unsigned int)num;
}

int hex_to_int(unsigned char hex) {
  return (int)hex;
}
	
int is_ressource_fun(int id_service){
//le train a en mémoire son parcourt : liste de services qu'il demande. Il faut donc une fonction pour déterminer si ce service recquiert l'accés à une des ressources critiques identifiées
	int is_critical [] = {50,60,52,62,32,42,12,9,13,33,43,27,28,29,53,63,11,31,7,37}; // ensemble des id des services qui appartiennent à l'une des 10 ressources
	int length = sizeof(is_critical) / sizeof(is_critical[0]); 
	int is_ressource = 0;
    	
    	for (int i = 0; i < length; i++) {
        	if (is_critical[i] == id_service) {
            		is_ressource = 1;
           		break;
        }
    }
    
    if (is_ressource) {
    	printf("%d se trouve dans la liste des ressources critiques.\n", id_service);
    } else {
        printf("%d ne se trouve pas dans la liste.\n", id_service);
    }
    
	return is_ressource;
}
	


int * ressource_2_infra(int id_ressource){
	int NB_MAX_REQ = 10;
	int liste_infra[NB_MAX_REQ];
	int infra_type;
	int infra_id;
	int infra;
	switch (id_ressource){
		case 1:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 2:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 3:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 4:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;;
			break;
		case 5:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 6:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 7:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 8:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 9:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		case 10:
			infra_type = PAkd;
			infra_id = 0;
			infra = convert_id_to_dico(infra_id, infra_type);
			liste_infra[0] = infra;
			break;
		}
	return liste_infra;
	}
	

int convert_id_to_dico(int id, int type){
	int dico;
	switch (type){
		case Tk:
			dico = id;
			break;
		case Tik:
			dico = id+40;
			break;
		case Akd:
			dico = id;
			break;
		case Tjkd:
			dico = id+30;
			break;
		case Tjkb:
			dico = id+40;
			break;
		case PAkd:
			dico = id+50;
			break;
		case PAkb:
			dico = id+60;
			break;
		case Akb:
			dico = id+20;
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

int requete_xway(int id_requete, int id_train, int id_trame, unsigned char XWAY_HEXA_TRAIN, int type, int sock_fd){
	unsigned char* message = (unsigned char*)malloc(27 * sizeof(unsigned char));  // Allocate memory for the XWAY message
	
	unsigned char id_req_hexa = int_to_hex(convert_id_to_dico(id_requete, type));
	unsigned char id_train_hexa = int_to_hex(id_train);
	unsigned char id_trame_hexa = int_to_hex(id_trame);
	unsigned char mot_write_hexa = int_to_hex(mot_ecriture(id_train_hexa));
	
	message[0] = 0x00;
	message[1] = 0x00;
	message[2] = 0x00;
	message[3] = 0x01;
	message[4] = 0x00;
	message[5] = 0x14;
	message[6] = 0x00;
	message[7] = 0xF1;
	message[8] = XWAY_HEXA_TRAIN; //Adresse X_Way
	message[9] = 0x10; //Adresse X_Way recepteur
	message[10] = 0x14;
	message[11] = 0x10;
	message[12] = 0x09;
	message[13] = id_trame;
	message[14] = 0x37;
	message[15] = 0x06;
	message[16] = 0x68;
	message[17] = 0x07;
	message[18] = mot_write_hexa;
	message[19] = 0x00;
	message[20] = 0x03;
	message[21] = 0x00;
	message[22] = XWAY_HEXA_TRAIN;
	message[23] = 0x00;
	if (type == Tk || type == Tik){
		message[24] = id_req_hexa;
		message[25] = 0x00;
		message[26] = 0xFF;
		message[27] = 0xFF;}
	else{
		message[24] = 0xFF;
		message[25] = 0xFF;
		message[26] = id_req_hexa;
		message[27] = 0x00;}
	
	printf("le message envoyé par le train vers l'API est : ");
	afficher_trame(message,28);
	size_t message_size = strlen((char*)message);
        printf("message de taille %ld\n",message_size);
        write(sock_fd, message, message_size);
   	printf("Message envoyé au train %d\n",2);
   	return 1;
}


int creation_message_vers_g2r(int message_type, int id_train, int id_service,int sock_fd, int id_ressource){
	printf("Création d'un message de type %d par le train d'id %d \n", message_type, id_train);
	int NB_OCTETS = 3;
	unsigned char message [NB_OCTETS]; // Allocate memory for the message
	switch (message_type){
		case 1:	
			// Convert train_id and service_id to hex strings and store in message
			message[0] = int_to_hex(message_type);
			//printf("message_0 = %hx\n",message[0]);
			message[1] = int_to_hex(id_train);
			message[2] = int_to_hex(id_service);
	
			break;
		case 3:		
			message[0] = int_to_hex(message_type);
			//printf("message_0 = %hx\n",message[0]);
			message[1] = int_to_hex(id_train);
			message[2] = int_to_hex(id_ressource); //le message dde confirmation contient l'ID de la ressource rendue	
			break;
		default:
			
			printf("Pas le bon type de message");
			exit(EXIT_FAILURE);
		}
	printf("le message envoyé par le train %d vers le G2R est : ",id_train);
	afficher_trame(message,NB_OCTETS);
	size_t message_size = strlen((char*)message);
        printf("message de taille %ld\n",message_size);
        write(sock_fd, message, message_size);
	return(0);
}

void afficher_trame(unsigned char* trame,int taille) {
	printf("la taille est de : %i\n",taille);
    	int i;
    	for (i = 0; i < taille; i++) {
        	printf("%04x ", trame[i]);
    	}
    	printf("\n");
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



int *lect_req_g2r(unsigned char* message_recu, int message_size){
	//cette fonction lit la requête reçue et en extrait les informations utilisées: type de message (req ou ack), l'ensemble des id_service faisant partis de la ressource et id_train qui le demande. L'id sera ensuite utilisé dans la fonction service_to_ressource
        printf("Le message recu contient %i octets\n",message_size);
        
        int* liste_param_recus = malloc(message_size * sizeof(int));
        
        
        for (int i=0;i<message_size;i++){
        	
        	liste_param_recus[i] = hex_to_int((unsigned char)message_recu[i]);
		printf("Message_content : %d\n",liste_param_recus[i]);
		}
        return liste_param_recus;
	
}

