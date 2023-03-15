#include <stdio.h>
#include "../include/lib_g2r.h"
#include "../include/lib_train.h"
int main(){
	printf("Hello world\n");
	int message_type = 18;
	int id_train = 1;
	float pos = 3.5;
	float speed = 30.3;
	int num_services = 3;
	int services[] = {1,3,7};
	int id_zone_suivi = 2;
	int id_serv_ok = 3;
	int sock_fd = 3;
	G2RMessage_envoi train_message;
	char * message = creation_message_vers_train(message_type, id_train,pos,speed,num_services,services,id_zone_suivi,id_serv_ok,sock_fd);
	lect_req_g2r(message, &train_message);
	return 0;
	}
