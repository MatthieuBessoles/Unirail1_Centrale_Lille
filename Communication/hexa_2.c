#include <stdio.h>
#include <stdlib.h>

unsigned char* train_and_resource_to_hex(unsigned int train_id, unsigned int resource_id);
unsigned char * lecture_trame(unsigned char * message);

int main() {
  // IDs of the train and resource to convert to hex
  unsigned int train_id = 1234;
  unsigned int resource_id = 5678;

  // Convert train and resource IDs to hex and store in message
  unsigned char* message = train_and_resource_to_hex(train_id, resource_id);

  // Print the hex message
  printf("Hex message: %s\n", message);
  long id_ress_recu = lecture_trame(message);
  printf("id_reçu = %ld", id_ress_recu);
  // Free the memory allocated for the message
  free(message);
  return 0;
  }

unsigned char* train_and_resource_to_hex(unsigned int train_id, unsigned int resource_id) {
  unsigned char* message = (unsigned char*)malloc(5 * sizeof(unsigned char));  // Allocate memory for the message

  // Convert train_id and resource_id to hex strings and store in message
  sprintf((char*)message, "%04X%04X", train_id, resource_id);

  return message;
}

unsigned char * lecture_trame(unsigned char * message){

	printf("mon message recu = %s", message);
	int * id_ressource = malloc(sizeof(int));
	sprintf((int*)id_ressource, "%2d",message[2]);
	return id_ressource;
	}
	

