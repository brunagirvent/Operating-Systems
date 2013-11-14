/* 7ymedio.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
    int status;
	int pganadasC=0;
	int numpartidas=atoi(argv[1]);
	
    if (argc != 2) {
	    fprintf(stderr, "USO: %s N\n",argv[0]);
	    exit(0);
    }

    printf("\n***Inicio del juego con %d partidas***\n\n", numpartidas);

    int i=0;
    int pid=1;

    for (i=0;i<numpartidas;i++){
    	switch(pid=fork()) {
		case -1: {
			printf("Error al crear al hijo\n");
			return(-1);
			}

		case 0: {
			printf("\n\n--------------- Inicio de la partida %d ----------------\n",i+1);
			//printf("\nDireccion de partidasc %p\n", &partidasC);
			execl("/Users/skiria/Dropbox/UPC/Ariso2/practicaProcessos/codis/partida","partida",NULL);
			exit(0);
			break;
			}

		default: {
			wait(&status); //Esperamos a que termine la partida y nos diga quien ha ganado
			if (status/256==1)pganadasC++; //Si ha ganado el Croupier
			}		
		}
	} 

	printf("\nEl croupier ha ganado %d partidas\n", pganadasC);


    return(0);
}
