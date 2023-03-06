#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Adresse IP du serveur
#define SERVER_PORT 8888 // Port du serveur
#define TRAIN_ID 1 // Numéro du train
#define RESOURCE_ID 3 // Numéro de la ressource demandée

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char message[50], server_reply[2000];

    // Création de la socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Erreur: création de socket a échoué");
    }

    // Configuration de l'adresse du serveur
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erreur: connexion au serveur a échoué");
        return 1;
    }

    // Création du message contenant l'ID du train et l'ID de la ressource demandée
    sprintf(message, "Train %d demande la ressource %d", TRAIN_ID, RESOURCE_ID);

    // Envoi du message au serveur
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Erreur: envoi de message a échoué");
        return 1;
    }

    printf("Message envoyé: %s\n", message);

    // Réception de la réponse du serveur
    if (recv(sock, server_reply, 2000, 0) < 0) {
        perror("Erreur: réception de réponse a échoué");
        return 1;
    }

    printf("Réponse reçue: %s\n", server_reply);

    close(sock);
    return 0;
}

