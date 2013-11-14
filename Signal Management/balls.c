#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int add_ball=0, reset_ball=0;

void ras_sigusr1(int signv) {
	if (signv!=SIGUSR1) exit(0);
	add_ball++;
}

void ras_sigusr2(int signv) {
	if (signv!=SIGUSR2) exit(0);
	reset_ball++;
}


int main(int argc, char *argv[])
{

	int fdpipePH[2],fdpipeHP[2], v=0,i=0,pid,x,max;
	struct sigaction sig1,sig2;

	//control numero parametros entrada
	if( argc != 2 ){
		fprintf(stderr,"\n\bUso:%s [int]\n",argv[0]);	
		exit(1);
	}
	max=atoi(argv[1]);

	//SIGUSR1: añadir bola
	sig1.sa_handler=ras_sigusr1;
	sigemptyset(&sig1.sa_mask);
	sig1.sa_flags=0;
	sigaction(SIGUSR1,&sig1,NULL);

	//SIGUSR2: reset
	sig2.sa_handler=ras_sigusr2;
	sigemptyset(&sig2.sa_mask);
	sig2.sa_flags=0;
	sigaction(SIGUSR2,&sig2,NULL);

 	//crear pipes
	pipe(fdpipePH);
	pipe(fdpipeHP);
	
	pid=fork();

		switch (pid) {
			case -1: { 
					fprintf(stderr,"Error fork\n"); 
					exit(0);
					 }

			case 0: { //HIJO
					close(fdpipePH[1]);	
					close(fdpipeHP[0]); 

					while(1) {

						//lectura
						x=read(fdpipePH[0],&v,sizeof(int));
						while(x==-1 && errno==EINTR) x=read(fdpipePH[0],&v,sizeof(int)); //control error

						//incremento
						sleep(1);
						v++;	

						//ATENDER SIGNALS
						if(add_ball>0){
							write(fdpipeHP[1],&i,sizeof(int));
							add_ball--;
						}
						if(reset_ball>0){
							v=0;
							reset_ball--;
						}		

						//final (ultima iteracion)
						if (v>max) {
							write(fdpipeHP[1],&v,sizeof(int));
							exit(1);
						}

						//Escritura
						fprintf(stderr,"H:%d ",v); //por stdout hay un retardo
						write(fdpipeHP[1],&v,sizeof(int));
					  }

						
					break;
					}

			default: { //PADRE
					close(fdpipePH[0]);	
					close(fdpipeHP[1]);

					fprintf(stderr,"Inicio del juego\n");	

					while(1) {

						//escritura
						fprintf(stderr,"P:%d ", v);
						write(fdpipePH[1],&v,sizeof(int));

						//lectura
						x=read(fdpipeHP[0],&v,sizeof(int));
						while(x==-1 && errno==EINTR) x=read(fdpipeHP[0],&v,sizeof(int)); //control error

						//incremento
						sleep(1);
						v++;

						//final (ultima iteracion)
						if(v>max) {
							if(v%max==1) write(fdpipePH[1],&v,sizeof(int));
							waitpid(pid,NULL); //esperar al hijo
							fprintf(stderr,"\nFin del juego\n");
							exit(1);
						}

						//ATENDER SIGNALS
						if(add_ball>0){
							write(fdpipePH[1],&i,sizeof(int));
							add_ball--;
						}

						if(reset_ball>0){
							v=0;
							reset_ball--;
						}

			      }//while
	  			}//default	
		}//switch
}

