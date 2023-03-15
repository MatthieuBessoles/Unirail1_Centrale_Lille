/* ------------------------------------------------------------------------ */
/*		S T R U C T U R E S   D E S  E N V O I S				*/
/* ------------------------------------------------------------------------ */
typedef struct {
    int message_type;
    int id_train;
    int num_services;
    float speed;
    float dist;
    int * services_ok;
} G2RMessage_envoi;

/* ------------------------------------------------------------------------ */
/*		S T R U C T U R E S   D E S  A R G U M E N T S				*/
/* ------------------------------------------------------------------------ */
typedef struct {
    int sockfd;
    int message_type;
} thread_argument;
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
} TrainMessage_reception;

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
void lect_req_train(char* message_recu,TrainMessage_reception* train_message);
int service_to_ressource(int id_service);
int * ressource_2_list_services(int id_ressource);
char* creation_message_vers_train(int message_type, int id_train, float dist, float speed,int num_services_ok, int* services,int id_zone_suivi, int id_serv_ok,int sock_fd);

void handle_message_type(void* args);
void lancement_thread(TrainMessage_reception* train_message, int sock);

