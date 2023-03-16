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


typedef struct {
    int message_type;
    int id_train;
    float position;
    float speed;
    int id_service_1;
    int id_service_2;
    int id_service_3;
} TrainMessage_envoi;

typedef struct {
    int message_type;
    int id_train;
    int num_services;
    float speed;
    float dist;
    int * services_ok;
} G2RMessage_reception;

/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */


void lect_req_g2r(char* message_recu, G2RMessage_reception* g2r_message, int sock_fd);
int convert_id_to_dico(int id, int type);
int mot_ecriture(unsigned char id_train);
void creation_message_vers_g2r(int message_type, int id_train, float pos, float speed,int id_serv_1, int id_serv_2, int id_serv_3,int sock_fd);
int connect_to_server(char *remoteip, int remoteport);
void close_socket(int sock) ;
