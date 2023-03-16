#include <semaphore.h>
/* ------------------------------------------------------------------------ */
/*		S T R U C T U R E S   D E S  E N V O I S				*/
/* ------------------------------------------------------------------------ */
typedef struct {
    int message_type;
    int id_train;
    int num_services; //nombre de services accepté
    float speed;
    float dist;
    int id_ressource;
    int * services_ok;
} G2RMessage_envoi;


/* ------------------------------------------------------------------------ */
/*		S T R U C T U R E S   D E S  R E P O N S E S				*/
/* ------------------------------------------------------------------------ */
typedef struct {
    int message_type;
    int id_train;
    float position;
    float speed;
    int id_service_1;
    int id_service_2;
    int id_service_3;
    int sockfd;
    int id_ressource;
} TrainMessage_reception;
/* ------------------------------------------------------------------------ */
/*		S T R U C T U R E S   D E S  A R G U M E N T S				*/
/* ------------------------------------------------------------------------ */
typedef struct {
    
    int message_type;
    int id_train;
    float position;
    float speed;
    int id_service_1;
    int id_service_2;
    int id_service_3;
    int sockfd;
    int id_ressource;
} thread_argument;

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
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */
//pour l'utilisation de CHECK_IF
#define VITESSE_URGENCE 0
#define VITESSE_APPROCHE 10
#define VITESSE_AIGUILLAGE 20
#define VITESSE_VIRAGE 30
#define VITESSE_MAX 45
#define NUM_RESSOURCES 9


/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */
void lect_req_train(char* message_recu, TrainMessage_reception* train_message, int sock_fd);
int service_to_ressource(int id_service);
float vitesse_to_service(int id_service);
void autorisation(int message_type, int train_id, float pos, float speed, int id_service_1, int id_service_2, int id_service_3, int sockfd,int id_ressource);
int * ressource_2_list_services(int id_ressource);
void creation_message_vers_train(int message_type, int id_train, float dist, float speed,int num_services_ok, int* services,int id_zone_suivi, int id_serv_ok,int sock_fd,int id_ressource);
void check_position(int message_type, int train_id, float pos, float speed, int id_service_1, int id_service_2, int id_service_3, int sockfd);
void handle_message_type(void* args);
void lancement_thread(TrainMessage_reception* train_message, int sock);

