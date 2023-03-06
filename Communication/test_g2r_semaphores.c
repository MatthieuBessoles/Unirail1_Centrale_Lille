#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <semaphore.h>

/* ------------------------------------------------------------------------ */
/*			C O N S T A N T E S     S Y M B O L I Q U E S				*/
/* ------------------------------------------------------------------------ */

#define NUM_RESOURCES 10

/* ------------------------------------------------------------------------ */
/*		V A R I A B L E S     G L O B A L E S						*/
/* ------------------------------------------------------------------------ */

// Déclaration des sémaphores pour les ressources
sem_t resource_semaphores[NUM_RESOURCES];

int main(){
	// Initialisation des sémaphores pour chaque ressource
    for (int i = 0; i < NUM_RESOURCES; i++) {
        sem_init(&resource_semaphores[i], 0, 1);
    }
    
