/****************************************************/
/* Application : ressource_manager                           */
/* Date : 18/02/2023                                */
/* Version : 2                                      */
/* Compilation : gcc ressource_manager_2.c -o ressource_manager_2          */
/* Usage : ./ressource_manager_2 */
/****************************************************/
/* ------------------------------------------------------------------------ */
/*			E N T Ê T E S    S T A N D A R D S						*/
/* ------------------------------------------------------------------------ */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>

/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */

#define NUM_RESOURCES 10
#define LOCALIP "127.0.0.1" //IP du PC gestionnaire de ressources
#define LOCALPORT 8888 //Port du processus gestionnaire de ressource
#define REMOTEPORT_T1_T3 3000 //T1/2 sont gérés sur le même PC
#define REMOTEPORT_T2_T4 2000 //T3/4 sont gérés sur le même PC
#define REMOTEIP_T1_T2 "127.0.0.1" //A MODIFIER
#define REMOTEIP_T3_T4 "127.0.0.1" //A MODIFIER
/* ------------------------------------------------------------------------ */
/*			M A C R O - F O N C T I O N S						*/
/* ------------------------------------------------------------------------ */
#define CHECKERROR(var,val,msg)  if (var ==val) {perror (msg);}



/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */

char lect_req(int sockfd, int newsock_idtrain, struct sockaddr * train_addr);

/* ------------------------------------------------------------------------ */
/*		V A R I A  B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */
// Déclaration des sémaphores pour les ressources
sem_t resource_semaphores[NUM_RESOURCES];


int main(int argc, char *argv[]) {
    //déclaration des sockets pour les 4 trains
    int sockfd, newsock_1, newsock_2, newsock_3,newsock_4;
    
    socklen_t clilen;
    char buffer_1[256], buffer_2[256],buffer_3[256],buffer_4[256],;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr_T1;
    struct sockaddr_in cli_addr_T2;
    struct sockaddr_in cli_addr_T3;
    struct sockaddr_in cli_addr_T4;
    int n;

    // Initialisation des sémaphores pour chaque ressource
    for (int i = 0; i < NUM_RESOURCES; i++) {
        sem_init(&resource_semaphores[i], 0, 1);
    }

    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erreur lors de la création du socket");
        exit(1);
    }

    // Initialisation des valeurs de la structure sockaddr_in
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(LOCALPORT);

    // Binding
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors du binding");
        exit(1);
    }

    // Écoute
    listen(sockfd, 5);
    clilen = sizeof(cli_addr_T1); //même taille peut importe le train

    while (1) {
    	//Lecture pour les 4 trains
    	char ressource_t1 = lect_req(char * buffer_1, int sockfd, int newsock_1, &cli_addr_T1);
        char ressource_t2 = lect_req(char * buffer_2, int sockfd, int newsock_2, &cli_addr_T2);
        char ressource_t3 = lect_req(char * buffer_3, int sockfd, int newsock_3, &cli_addr_T3);
        char ressource_t4 = lect_req(char * buffer_4, int sockfd, int newsock_4, &cli_addr_T4);
       
        // Traite la demande
        // ... traiter la demande ici ...

        // Déverrouille la ressource en relâchant le sémaphore
        sem_post(&resource_semaphores[resource_index]);
    }

    // Ferme la connexion
    close(newsockfd);
}

// Ferme les sémaphores
for (int i = 0; i < NUM_RESOURCES; i++) {
    sem_destroy(&resource_semaphores[i]);
    
}

return 0;
}
	


