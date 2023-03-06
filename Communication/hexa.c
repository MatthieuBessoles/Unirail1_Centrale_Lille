#include <stdio.h>
#include <stdlib.h>


unsigned char * creation_message(int message_type, int id_train, int id_ressource, int accepte);
void afficher_trame(unsigned char * trame);

void main(){
	
	unsigned int id_ressource = 9;
	unsigned int id_train = 2;
	unsigned int accepte = 0;
	unsigned int message_type = 3;
	 
	unsigned char * message = creation_message(message_type, id_train,id_ressource, accepte) ;
	afficher_trame(message);
	free(message);
}




unsigned char * creation_message(int message_type, int id_train, int id_ressource, int accepte){
	printf("Création d'un message de type %d par le train d'id %d \n", message_type, id_train);
	unsigned char* message = (unsigned char*)malloc(7 * sizeof(unsigned char));  // Allocate memory for the message
	switch (message_type){
		case 1:	
			// Convert train_id and resource_id to hex strings and store in message
  			sprintf((char*)message, "%04X%04X%04X",message_type , id_train, id_ressource);	
			break;
		case 2:	
			sprintf((char*)message, "%04X%04X%04X",message_type, id_train, accepte);
			break;
		case 3:
			printf("message de type 3\n");
			sprintf((char*)message, "%04X%04X%04X",message_type , id_train, id_ressource);			
			// il s'agit ici de la ressource libérée par le train
			break;
		case 4:		
			sprintf((char*)message, "%04X%04X%04X",message_type , id_train, id_ressource);	
			break;
		default:
			
			printf("Pas le bon type de message");
			exit(EXIT_FAILURE);
		}
	return(message);
}

void afficher_trame(unsigned char * trame) {
    printf("Hex message: %s\n", trame);
}



