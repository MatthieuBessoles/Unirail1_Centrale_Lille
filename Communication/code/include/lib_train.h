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




/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */


int *lect_req_g2r(unsigned char* message_recu, int message_size);
unsigned int int_to_hex(int num);
int hex_to_int(unsigned char hex);
int is_ressource_fun(int id_service);
int * ressource_2_infra(int id_ressource);
int convert_id_to_dico(int id, int type);
int mot_ecriture(unsigned char id_train);
int requete_xway(int id_requete, int id_train, int id_trame, unsigned char XWAY_HEXA_TRAIN, int type, int sock_fd);
int creation_message_vers_g2r(int message_type, int id_train, int id_service,int sock_fd, int id_ressource);
int connect_to_server(char *remoteip, int remoteport);
void afficher_trame(unsigned char* trame,int taille);
void close_socket(int sock) ;
