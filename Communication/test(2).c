/****************************************************/
/* Application : train                           */
/* Date : 02/02/2023                                */
/* Version : 1                                      */
/* Compilation : gcc train.c -o train          */
/* Usage : ./train [adrIPserv] [portServ]   [adrIPcli] */
/****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


#define MAXCAR  15
#define LOCALIP "10.31.125.104"
#define XWAY "1.4"
#define XWAY_HEXA 0x04

#define REMOTEIP "10.31.125.14"
#define REMOTEPORT 502

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}

void afficher_trame(unsigned char* trame);

void activer_troncon(unsigned char * message, unsigned char id_troncon, unsigned char id_train, unsigned char id_trame);

int main(char * argv[], int argc)
{ 
	int sd1; //descripteur de socket de dialogue
	struct sockaddr_in addrServ, addrCli;
	unsigned char req_received[MAXCAR];
	unsigned char thread_number = 10;
	int erreur, nbcar;
	int adrlg=sizeof(struct sockaddr_in);

	//Etape 1 - Creation de la socket
	sd1=socket(AF_INET, SOCK_STREAM, 0);
	CHECKERROR(sd1,-1, "Creation fail !!!\n");

	//Etape2 - Adressage du destinataire
	addrServ.sin_family=AF_INET;
	addrServ.sin_port=htons(REMOTEPORT);
	addrServ.sin_addr.s_addr=inet_addr(REMOTEIP);

	//Etape 3 - demande d'ouverture de connexion
	CHECKERROR(connect(sd1, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)),-1, "Connexion fail !!!\n");
	
	
	//Etape 4: Envoi de la requete
	//Demande activation tronçon 13
	//unsigned char message[] = {0x00,0x00,0x00,0x01,0x00,0x14,0x00,0xF1,0x04,0x10,0x14,0x10,0x09,0x10,0x37,0x06,0x68,0x07,0x01,0x00,0x03,0x00,0x04,0x00,0x0D,0x00,0xFF,0xFF};
	unsigned char message[] = {0x00,0x00,0x00,0x01,0x00,0x14,0x00,0xF1,'a','b',0x14,0x10,0x09,'c',0x37,0x06,0x68,0x07,'d',0x00,0x03,0x00,'e',0x00,'f',0x00,0xFF,0xFF};
	activer_troncon(message, 0x0D, 0x01, 0xEE);
	
	int message_size = sizeof(message);
	int bytes_sent = send(sd1,message,message_size,0);
	
	printf("Contenu de l'envoi: ");
	afficher_trame(&message);
	if (bytes_sent < 0){
		perror("erreur envoi");
	}
	
	
	//Etape 4: Reception acknowledge
	recvfrom(sd1,req_received, MAXCAR+1, 0, &addrServ, &adrlg);
	printf("Contenu de la reception: ");
	afficher_trame(req_received);
	
	
	printf("Attente deuxieme message");
	//Attente de la reception d'une trame
	int bytes_received = read(sd1,req_received, MAXCAR+1);
	if (bytes_received < 0) {
    	perror("Erreur reception");
	}
	else {
    	printf("Contenu de la reception: ");
    	afficher_trame(req_received);
	}
	
	unsigned char message_confirmation [] = {0x00,0x00,0x00,0x01,0x00,0x07,0x00,0xF1,0x13,0x10,0x14,0x10,0x19,req_received[13],0xFE};
	write(sd1,message_confirmation, MAXCAR+1);
	afficher_trame(message_confirmation);
	//Etape6 - on ferme la socket ce qui va liberer le port
	close(sd1);

	//passage en mode serveur

	return EXIT_SUCCESS;
}
void activer_troncon(unsigned char * message, unsigned char id_troncon, unsigned char id_train, unsigned char id_trame){
	
	message[8] = XWAY_HEXA; //Adresse X_Way
	message[9] = 0x10; //Adresse X_Way
	
	message[13] = id_trame;
	message[18] = id_train;
	message[22] = XWAY_HEXA;
	message[24] = id_troncon;
	
	
}

void afficher_trame(unsigned char* trame) {
	int taille = 8 + (int) trame[5];
    int i;
    for (i = 0; i < taille; i++) {
        printf("%02x ", trame[i]);
    }
    printf("\n");
}



