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


#define MAXCAR  80
#define LOCALIP "10.31.125.0"
#define REMOTEIP "10.31.125.14"
#define REMOTEPORT 502

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}

int main(char * argv[], int argc)
{
    
int sd1; //descripteur de socket de dialogue
struct sockaddr_in addrServ, addrCli;
char req_sent[MAXCAR+1];
char req_received[MAXCAR+1];
unsigned char thread_number = 10;
int erreur, nbcar;
int adrlg=sizeof(struct sockaddr_in);

//Etape 1 - Creation de la socket

sd1=socket(AF_INET, SOCK_STREAM, 0);

CHECKERROR(sd1,-1, "Creation fail !!!\n");

//Etape2 - Adressage du destinataire

addrServ.sin_family=AF_INET;

if (argc>=3)
{
    addrServ.sin_port=htons(atoi(argv[2]));
    addrServ.sin_addr.s_addr=inet_addr(argv[1]);
}
else
{
addrServ.sin_port=htons(REMOTEPORT);
addrServ.sin_addr.s_addr=inet_addr(REMOTEIP);
}

//Etape 3 - demande d'ouverture de connexion

CHECKERROR(connect(sd1, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)),-1, "Connexion fail !!!\n");

do
{

printf("CLIENT '%d'>", getpid());
//fgets(buff, MAXCAR,stdin);
//buff[strlen(buff)-1]='\0';
char req = strcat("00000001000A00F11E10141009","10");
char req_2 = strcat(req,"2406");
strcpy(req_sent,req_2); //On est 1.30 en ETHWAY ==> 1E (16+14) en hexa
nb_car_1=send(sd1,req_sent, strlen(req_sent), 0);
//printf("CLIENT '%d' : ", getpid());
recvfrom(sd1,req_received, MAXCAR+1, 0, &addrServ, &adrlg); 

//if (nbcar) printf(" '%s' \n", buff);
}

while (strcmp(buff, "FIN") && strcmp(buff, "fin"));
    
//Etape6 - on ferme la socket ce qui va liberer le port
close(sd1);

return EXIT_SUCCESS;
}


    
 














}

