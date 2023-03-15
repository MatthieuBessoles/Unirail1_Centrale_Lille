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


/* ------------------------------------------------------------------------ */
/*		V A R I A  B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */



int main() {
	int liste_parcourt[50] = {1,2,3, 81,70,78,77,13,104,105,7,8,9,92};/*à remplir avec le parcourt */

	//1. Connexion du train au G2R en TCP/IP
	int length_parcourt = 12;
	

	int sd_g2r = connect_to_server(SERVER_IP,SERVER_PORT);
	printf("[TRAIN 1]Bien connecté au serveur g2r\n");
	int type = 2;
	while(1){ //le train tourne en permanance sur son circuit
		int i; //index du service demandé dans liste_parcourt
		for(i = 0; i <length_parcourt; i++){
			//1. On vérifie si le service demandé est une ressource critique
			int mon_service = liste_parcourt[i];
			printf("[TRAIN 1]mon service demandé est : %d\n",mon_service);
			
			printf("Hello world\n");
			int message_type = 5;
			int id_train = 1;
			float pos = 3.5;
			float speed = 30.3;
			//int num_services = 3;
			int service_1 = 12;
			int service_2 = 14;
			int service_3 = 16;
			//int id_zone_suivi = 2;
			//int id_serv_ok = 3;
			int sock_fd = 3;
			TrainMessage_envoi train_message;
			char * message = creation_message_vers_g2r (message_type, id_train,pos,speed,service_1, service_2,service_3,sd_g2r);
			lect_req_train(message, &train_message);

    	} //fin du parcourt
    sleep(5);
    }

    close(sd_g2r);
    //close(sd_api);
    return 0;
}

