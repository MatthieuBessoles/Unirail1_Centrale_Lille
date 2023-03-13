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
#define API_IP "10.31.125.14" // Adresse IP de l'API
#define API_PORT 502 // Port du serveur G2R
#define TRAIN_ID 1 // Numéro du train
#define TRAME_ID 1
#define XWAY_HEXA_TRAIN 0x01 //train 1
#define demande_g2r 1 //premier message vers le G2R pour réservation de la ressource
#define restitution_g2r 3 //fin d'utilisation de la ressource du train vers g2R 


/* ------------------------------------------------------------------------ */
/*		V A R I A  B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */



int main() {
	int liste_parcourt[50] = {43,31,50,23,52,32,50,33,29,53,11,19};/*à remplir avec le parcourt */

	//1. Connexion du train au G2R en TCP/IP
	int length_parcourt = 12;
	

	int sd_g2r = connect_to_server(SERVER_IP,SERVER_PORT);
	printf("[TRAIN 1]Bien connecté au serveur g2r\n");
	//2. Connexion du train à l'API en XWAY
	//int sd_api = connect_to_server(API_IP,API_PORT);
	int type = 2;
	while(1){ //le train tourne en permanance sur son circuit
		int i; //index du service demandé dans liste_parcourt
		for(i = 0; i <length_parcourt; i++){
			//1. On vérifie si le service demandé est une ressource critique
			int mon_service = liste_parcourt[i];
			printf("[TRAIN 1]mon service demandé est : %d\n",mon_service);
			//2. Si oui : demande au G2R, sinon : demande directement à l'API
			if (is_ressource_fun(mon_service)){
				//1. Création et envoi du message à envoyer au G2R
				int accepte = 0;
				creation_message_vers_g2r(demande_g2r, TRAIN_ID, mon_service,sd_g2r,0);
				//attente passive tant que le g2r ne renvoie rien
				//3. Réception de la trame du g2r
				unsigned char* messagage_g2r_rep= malloc(15 * sizeof(unsigned char) * 2);
				int m = read(sd_g2r, messagage_g2r_rep, sizeof(messagage_g2r_rep));
    				if (m < 0) {
        				perror("Erreur lors de la lecture de la demande du client");
        				close(sd_g2r);
        				return NULL;
    				}
    	
    				 //l'ensemble des id des services à demande à l'API par le train
				
				//4. Lecture de cette réponse, il faut également récupérer l'ID de la ressource pour la restitution
				int reception_taille =strlen((char*)messagage_g2r_rep);
				printf("[TRAIN 1]Message reçu : ");
    				afficher_trame ( messagage_g2r_rep , reception_taille);
				//Dans la trame de retour se trouvent
				int *list_param_received = lect_req_g2r(messagage_g2r_rep, reception_taille);
				int message_type_received = list_param_received[0];
				int id_train_received = list_param_received[1];
				int number_services_received = list_param_received[2];
				int ressource_utilise = list_param_received[3];
				//4.1. On contrôle que c'est bien ce que l'on attendait de recevoir
				//Je ne sais pas encore comment agir dans ce cas.
				//4.2. On contrôle que c'est bien ce que l'on attendait de recevoir
				//5. Demande des services vers l'API
				
				/*for (int i = 0; i<number_services_received;i++){
					//5.1. Création et envoi de la trame XWAY vers l'API
					requete_xway(list_param_received[i], TRAIN_ID, TRAME_ID, XWAY_HEXA_TRAIN, type, sd_api); //A VOIR TYPE (AIGUILLAGE OU TRONCON)
				}*/
				printf("[TRAIN 1]je demande l'accession à l'API\n");
				sleep(3);
				//6. Restitution de la ressource
				creation_message_vers_g2r(restitution_g2r, TRAIN_ID, mon_service,sd_g2r,ressource_utilise);
				//7. Attente passive de la bonne restitution par le G2R et de la réception de la confirmation
				unsigned char* confirmation_g2r_rep= malloc(2 * sizeof(unsigned char) * 2); //2 octets
				int n = read(sd_g2r, confirmation_g2r_rep, sizeof(confirmation_g2r_rep));
    				if (n < 0) {
        				perror("Erreur lors de la lecture de la demande du client");
        				close(sd_g2r);
        				return NULL;
    				}
    				printf("[TRAIN 1]Message reçu : ");
    				afficher_trame(confirmation_g2r_rep, 2);
			}
				
			else {
				//1. Création et envoi de la trame XWAY à à l'API				
				printf("[TRAIN 1]réation et envoi de la trame XWAY à à l'API pour ce service non critique \n");
				//requete_xway(mon_service, TRAIN_ID, TRAME_ID, XWAY_HEXA_TRAIN, type, sd_api);
				
				}

    	} //fin du parcourt
    sleep(5);
    }

    close(sd_g2r);
    //close(sd_api);
    return 0;
}

