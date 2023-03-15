#include <stdio.h>
#include "../include/lib_train.h"
#include "../include/lib_g2r.h"
int main(){
	printf("Hello world\n");
	int message_type = 5;
	int id_train = 1;
	float pos = 3.5;
	float speed = 30.3;
	//int num_services = 3;
	int service_1 = 12;
	int service_2 = 14;
	int service_3 = 16;
	//int id_zone_suivi = 2;
	//int id_serv_ok = 3;
	int sock_fd = 3;
	TrainMessage_envoi train_message;
	char * message = creation_message_vers_g2r(message_type, id_train,pos,speed,service_1, service_2,service_3,sock_fd);
	lect_req_train(message, &train_message);
	return 0;
	}
