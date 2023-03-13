
/* ------------------------------------------------------------------------ */
/*		P R O T O T Y P E S    D E    F O N C T I O N S				*/
/* ------------------------------------------------------------------------ */

int creation_message_vers_train(unsigned char * message,int message_type, int id_train, int* list_services, int id_service, int accepte, int sock_fd,int id_ressource);
void afficher_trame(unsigned char* trame,int taille) ;
int hex_to_int(unsigned char hex) ;
unsigned int int_to_hex(int num);
void generate_hex_message(int* int_list, int list_size);
int * ressource_2_list_services(int id_ressource);
int service_to_ressource(int id_service);
int *lect_req(unsigned char* message_recu, int message_size);
