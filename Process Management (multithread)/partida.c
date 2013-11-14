/* ----------------------------------------------------------------------- */

/* partida.c: Juego del 7 y medio			                                   */

/* ----------------------------------------------------------------------- */

/* El juego consiste en ir pidiendo cartas intentando sumar 7.5 puntos     */
/* sin pasarse. Gana el jugador que más se acerca a 7.5.		   */
/* Se utiliza una baraja española. Sota, caballo y rey valen medio punto.  */
/* Las demás cartas puntúan según su valor numérico (de 1 a 7).            */

/* ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#define PALOS    4
#define NUMEROS 10
#define CARTAS  (NUMEROS*PALOS)

#define TRUE 1
#define FALSE 0

//Estructura tipo carta
typedef struct
{
    char *palo;
    char *nombre;
    int  valor;
}carta;

/* Baraja ordenada: primero las 10 cartas de Oros, luego las 10 	  */
/* de Copas, las 10 de Espadas y finalmente las 10 de Bastos.		  */
/* Para no gestionar valores decimales, el valor de cada carta se guarda  */
/* multiplicado por 2 							  */

carta baraja[CARTAS]={ 
 { "Oros", "as", 2 }, { "Oros", "2", 4 },
 { "Oros", "3", 6 }, { "Oros", "4", 8 },
 { "Oros", "5", 10 }, { "Oros", "6", 12 },
 { "Oros", "7", 14 }, { "Oros", "sota", 1 },
 { "Oros", "caballo", 1 }, { "Oros", "rey", 1 },

 { "Copas", "as", 2 }, { "Copas", "2", 4 },
 { "Copas", "3", 6 }, { "Copas", "4", 8 },
 { "Copas", "5", 10 }, { "Copas", "6", 12 },
 { "Copas", "7", 14 }, { "Copas", "sota", 1 },
 { "Copas", "caballo", 1 }, { "Copas", "rey", 1 },

 { "Espadas", "as", 2 }, { "Espadas", "2", 4 },
 { "Espadas", "3", 6 }, { "Espadas", "4", 8 },
 { "Espadas", "5", 10 }, { "Espadas", "6", 12 },
 { "Espadas", "7", 14 }, { "Espadas", "sota", 1 },
 { "Espadas", "caballo", 1 }, { "Espadas", "rey", 1 },

 { "Bastos", "as", 2 }, { "Bastos", "2", 4 },
 { "Bastos", "3", 6 }, { "Bastos", "4", 8 },
 { "Bastos", "5", 10 }, { "Bastos", "6", 12 },
 { "Bastos", "7", 14 }, { "Bastos", "sota", 1 },
 { "Bastos", "caballo", 1 }, { "Bastos", "rey", 1 }, };

void intercambiar(carta *c1, carta *c2);
void barajar(carta baraja[CARTAS]);

int main(int argc, char *argv[])
{
    int actual, prev, pts_j, pts_c, sigue_j, sigue_c;
  
    actual = 0;
    prev = 0;
    pts_j = 0;
    pts_c = 0;
    sigue_j = TRUE;
    sigue_c = TRUE;

    barajar(baraja); //BARAJAR CARTAS
	
	
	//Bucle hasta acabar la partida (Jugador y croupier no sigan)
    do {
		
 	  /* Jugador */
	  if(sigue_j){
		
		  //COJER OTRA CARTA
		  //si croupier no se ha pasado (>7.5p) i jugador tiene menos de 5p o más que el croupier 
		  if((pts_c <= 15) && ((pts_j < pts_c)||(pts_j < 10))){
	      prev=pts_j;
			  
		  //Jugador coje una carta
	      pts_j += baraja[actual].valor;
	      printf("\n-Jugador- carta: \"%s de %s\", prev:%2.1f actual:%2.1f\n",baraja[actual].nombre,baraja[actual].palo,prev/2.0,pts_j/2.0);
		   
		      //Si jugador tiene > 7.5 --> se HA PASADO (No sigue)
              if (pts_j > 15){
                    printf("Jugador, te has pasado !!!\n");
                    sigue_j = FALSE;
              }
		   
				//Si jugador tiene un 7.5 --> se PLANTA (No sigue)
			  else if (pts_j == 15){
                    printf("Jugador, se planta con 7.5\n");
                    sigue_j = FALSE;
			  }	
			  //Si jugador <7.5 --> SIGUE
			  else  actual++;
	     }
		
		  //PLANTARSE
		//Si el croupier se ha pasado o jugador tiene >5puntosmayor a puntuación del croupier -->Se PLANTA (No sigue)
	    else{
                    printf("Jugador, decide plantarse.\n");
		    sigue_j = FALSE;
	        } 
	    }
		
		
 	  /* Croupier */
	  if(sigue_c){	
		  //COJER OTRA CARTA
           if((pts_j <= 15) && ((pts_c < pts_j)||(pts_c < 10))){
	      prev=pts_c;
		  //Cojer una carta
	      pts_c += baraja[actual].valor;
	      printf("\n-Croupier- carta: \"%s de %s\", prev:%2.1f actual:%2.1f\n",baraja[actual].nombre,baraja[actual].palo,prev/2.0,pts_c/2.0);
             
			   if (pts_c > 15){
                    printf("Croupier se pasa.\n");
                    sigue_c = FALSE;
              }
			   else  actual++;
			}
		  
		   //PLANTARSE
		  else{
			  printf("Croupier, se planta.\n");
		     sigue_c = FALSE;
		} 
		  
		  
	  }
          printf("\n- Jugador: %2.1f puntos\n", pts_j/2.0);
          printf("- Croupier: %2.1f puntos\n", pts_c/2.0);
		
    } while(sigue_j || sigue_c);

	
    /* Resultado de la partida */
	//Jugador gana si pts_c<PTS_J<7.5 y pts_c<7.5(no se ha pasado)
    if((pts_j <= 15) && ((pts_j > pts_c) || (pts_c > 15))) {
		printf("\nEnhorabuena Jugador: has ganado la partida !!!\n");
		return(0); //devolvemos 0 para que el padre (7ymedio) sepa que ha ganado el jugador
	}
    else {
		printf("\nJugador: otra vez sera...\n");
		return(1); //devolvemos 1 para que el padre (7ymedio) sepa que ha ganado el croupier
	}

    exit(0);
}

//Función intercambia dos objetos de tipo carta
void intercambiar (carta *c1, carta *c2)
{
    carta aux;
	//intercambio
    aux = *c1;
    *c1 = *c2;
    *c2 = aux;
}

//Función que simula el efecto de barajar las cartas de la baraja
void barajar(carta baraja[CARTAS])
{
  int veces,i,c1,c2;

  srand(getpid());
  veces=rand() % 100; //veces que se intercambiaran dos cartas dentro de la baraja
	
	for (i=0; i<veces; i++) {
      c1=rand() % CARTAS; //carta aleatoria entre 0 y 39 (posición dentro de la baraja)
      c2=rand() % CARTAS; //carta aleatoria entre 1 y 40 (posición dentro de la baraja)
      intercambiar( &(baraja[c1]), &(baraja[c2]) ); //intercambio de cartas en la baraja=barajar
  }
}

