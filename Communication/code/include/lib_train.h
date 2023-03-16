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
#include <signal.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
    int message_type;
    int id_train;
    float position;
    float speed;
    int id_service_1;
    int id_service_2;
    int id_service_3;
    int id_ressource;
} TrainMessage_envoi;

typedef struct {
    int message_type;
    int id_train;
    int num_services;
    float speed;
    float dist;
    int id_ressource;
    int * services_ok;
    
} G2RMessage_reception;
/* ------------------------------------------------------------------------ */
/*		S T R U C T U R E S   D E S  A R G U M E N T S				*/
/* ------------------------------------------------------------------------ */
typedef struct {
    int id_train;
    int* trajet;
    int sockfd;
} thread_argument_train;
/* ------------------------------------------------------------------------ */
		/* D É F I N I T I O N S  D E  T Y P E S */
/* ------------------------------------------------------------------------ */
typedef void * (* pf_t)(void *) ;
/* ------------------------------------------------------------------------ */
/*			M A C R O - F O N C T I O N S						*/
/* ------------------------------------------------------------------------ */
#define CHECKERROR(var,val,msg)  if (var ==val) {perror (msg);}

# define CHECK_T(status , msg)						\
	if (0 != ( status )) {						\
		fprintf (stderr , " pthread erreur : %s\n", msg) ; 	\
		exit ( EXIT_FAILURE ) ;					\
	}

/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */


void lect_req_g2r(char* message_recu, G2RMessage_reception* g2r_message, int sock_fd);
int convert_id_to_dico(int id, int type);
int mot_ecriture(unsigned char id_train);
void creation_message_vers_g2r(int message_type, int id_train, float pos, float speed,int id_serv_1, int id_serv_2, int id_serv_3,int sock_fd, int id_ressource);
int connect_to_server(char *remoteip, int remoteport);
void init_timer() ;
void send_position(int signum);
void close_socket(int sock) ;
void lancement_thread_ppx(thread_argument_train* args);
void gestion_position(void* args);
void gestion_demande(void* args);
