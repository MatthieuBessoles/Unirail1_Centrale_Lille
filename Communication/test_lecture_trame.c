#include <stdlib.h>
#include <stdio.h>


int hex_to_int(unsigned char hex);
int *lect_req(unsigned char* message_recu, int message_size);

int main(){
	unsigned char hexa[] = {0x02,0x03,0x3c,0xFF,0xFE};
	int message_recu_size = sizeof(hexa)/(sizeof(hexa[0]));
	printf("Le message recu contient %i octets\n",message_recu_size);
	int *trame_recue=lect_req(hexa,message_recu_size);
	
	return 0;
	}
int hex_to_int(unsigned char hex) {
  return (int)hex;
}

int *lect_req(unsigned char* message_recu, int message_size){
	//cette fonction lit la requête reçue et en extrait les informations utilisées: type de message (req ou ack), id_service demandé et id_train qui le demande. L'id sera ensuite utilisé dans la fonction service_to_ressource
        printf("Le message recu contient %i octets\n",message_size);
        int id_service_demande;
        int id_train_demandeur;
        int message_type;
        
        int liste_param_recus [message_size];
        
        
        for (int i=0;i<message_size;i++){
        	
        	liste_param_recus[i] = hex_to_int((unsigned char)message_recu[i]);
		printf("Message_content : %d\n",liste_param_recus[i]);
		}
        return liste_param_recus;
	
}
