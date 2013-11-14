#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fdpipePH[2],fdpipeHP[2], v=0,i=0,pid,max;

	//control numero parametros entrada
	if( argc != 2 ){
		fprintf(stderr,"\n\bUso:%s [int]\n",argv[0]);	
		exit(1);
	}
	max=atoi(argv[1]);

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
						read(fdpipePH[0],&v,sizeof(int)); 
						sleep(1); 
						v++; 

						//control final ultima iteracion
						if (v>max) {
							write(fdpipeHP[1],&v,sizeof(int));
							exit(1);
						}

						fprintf(stderr,"H:%d ",v); //salida por stderr pk stdout tiene retardo
						write(fdpipeHP[1],&v,sizeof(int));
					  }
					break;
					}

			default: { //PADRE
					close(fdpipePH[0]);	
					close(fdpipeHP[1]);
					printf("Inicio del juego\n");	

					while(1) {
						fprintf(stderr,"P:%d ", v);//salida por stderr pk stdout tiene retardo 
						write(fdpipePH[1],&v,sizeof(int));
						read(fdpipeHP[0],&v,sizeof(int));
						sleep(1); 
						v++; 	

						//control ultima iteracion
						if(v>max) {
							if(v%max==1) write(fdpipePH[1],&v,sizeof(int));
							waitpid(pid,NULL); //esperar al hijo
							fprintf(stderr,"\nFin del juego\n");
							exit(1);
						}

			      }//while
	  			}//default	
		}//switch
}

