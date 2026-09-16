/* IMPORTACIÓN DE LIBRERÍAS */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <xc.h>

/* DEFINICIÓN DE VOCALES CON TILDE Y EÑES */
unsigned char a_tilde = 160;
unsigned char e_tilde = 130;
unsigned char i_tilde = 161;
unsigned char o_tilde = 162;
unsigned char u_tilde = 163;
unsigned char A_tilde = 181;
unsigned char E_tilde = 144;
unsigned char I_tilde = 214;
unsigned char O_tilde = 224;
unsigned char U_tilde = 233;
unsigned char enie = 164;
unsigned char Enie = 165;
unsigned char signoDePreguntaInvertido = 168;
unsigned char espacio = 32;
/* DEFINICIONES DE VARIABLES UNIVERSALES */
uint8_t mazoCompleto[4][15];
uint8_t mazoPorPalo[15] = { 'A', 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 'J', 'Q', 'K' };
//El J, Q y K valen 10
//El Az vale 1 u 11, dependiendo la suma de cartas que se tenga. Si se sumando 11 pasara 21, vale 1. Si no, vale 11.
uint8_t cantidadDeJugadores = 1;
uint8_t cantidadDeCartas_Casa[6] = { 0, 0, 0, 0, 0, 0 };
uint8_t cantidadDeCartas_Jugador[6] = { 0, 0, 0, 0, 0, 0 };
uint8_t cartasEnMazo_Casa[6][20];
uint8_t cartasEnMazo_Jugadores[6][20];
uint8_t indiceDeCartas_Casa[6] = { 0, 0, 0, 0, 0, 0 };
uint8_t indiceDeCartas_Jugadores[6] = { 0, 0, 0, 0, 0, 0 };
uint8_t Separador[5] = "\t*\t";
uint8_t separador[150] = "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = |\t*\t";
//unsigned char separador[150] = "| = = = = = = = = = |\t*\t Perdiste :(\n| = = = = = = = = = |\t*\t";

/* DEFINICIONES DE FUNCIONES */
//void input(unsigned char*, unsigned char);
void iniciarJuego(void);
uint8_t pedirCarta(void);
void mostarCarta(uint8_t);
void finDelJuego(void);
uint8_t randomNumber(uint8_t);

/* REGLAS */
/*
 * Hay una casa por cada jugador
 * Los valores de las cartas de los jugadores se suman.
 * El objetivo de los jugadores es sumar 21 puntos.
 * Los jugadores pueden dejar de jugar antes de llegar a 21 puntos sin perder.
 * Si un jugador se pasa de 21 puntos, pierde.
 * Si un jugador tiene exactamente 21 puntos, se dice "BLACKJACK" y gana autom�ticamente a la casa.
 * Después de que el jugador termine de pedir sus cartas, reci�n entonces la casa empieza a pedir.
 *    1. Primero revela la que estaba oculta.
 *    2. Después sí o sí pide una carta (teniendo 3 en total).
 *    3. La casa puede pedir cuantas cartas quiera.
 *      a. Si la casa se pasa de 21 puntos, pierde.
 *      b. Entre la casa y el jugador: el que tenga más puntos sin pasarse de 21 gana.
 * /

/* PASO A PASO */
/*
 * La casa saca dos cartas por jugador: muestra una y oculta la otra
 * A cada jugador se le dan dos cartas y todos las pueden ver
 * En cada turno del jugador, el jugador pide una carta (alternativamente puede pedir la cantidad que quiera)
 * El jugador puede dejar de pedir cartas en cualquier momento, habilitando a la casa a pedir cartas y terminar el juego.
 */

int main() {
    unsigned char yaSeIngresoUnaCantidadDeJugadores = 0;

    printf("%cCu%cntos jugadores hay? \n\n", signoDePreguntaInvertido, a_tilde);
    printf("\tCantidad de jugadores: ", a_tilde);
    do{
        scanf("%d", &cantidadDeJugadores);
        fflush(stdin);
        if(yaSeIngresoUnaCantidadDeJugadores==0){
            printf("\n\n");
            yaSeIngresoUnaCantidadDeJugadores++;
        }
        if(cantidadDeJugadores>6){
            printf("\n\tNo se permiten m%cs de seis jugadores.\n\t\tVolv%c a intentar: ", a_tilde, e_tilde);
        }
    }   while(cantidadDeJugadores>6);

    iniciarJuego();

    return 0;
}

void iniciarJuego(void){
    uint8_t auxIndex;

    //Reinicia el registro del mazo
    for(uint8_t auxIndiceMazo1=0; auxIndiceMazo1<4; auxIndiceMazo1++){
        for(uint8_t auxIndiceMazo2=0; auxIndiceMazo2<15; auxIndiceMazo2++){
            mazoCompleto[auxIndiceMazo1][auxIndiceMazo2];
        }
    }

    //Reinicia el mazo de cartas de cada casa
    for(uint8_t auxIndiceMazo1=0; auxIndiceMazo1<4; auxIndiceMazo1++){
        for(uint8_t auxIndiceMazo2=0; auxIndiceMazo2<15; auxIndiceMazo2++){
            cartasEnMazo_Casa[auxIndiceMazo1][auxIndiceMazo2];
        }
    }

    //Reinicia el mazo de cartas de cada jugador
    for(uint8_t auxIndiceMazo1=0; auxIndiceMazo1<4; auxIndiceMazo1++){
        for(uint8_t auxIndiceMazo2=0; auxIndiceMazo2<15; auxIndiceMazo2++){
            cartasEnMazo_Jugadores[auxIndiceMazo1][auxIndiceMazo2];
        }
    }

    //La casa de cada jugador saca dos cartas para sí misma
    for(uint8_t casaActual=0; casaActual<cantidadDeJugadores; casaActual++) {
        /*
        //Si el espacio en cartasEnMazo_Casa indicado por el índice actual (indiceDeCartas_Casa) no está vacío, se incrementa en 1 al índice actual
        while(cartasEnMazo_Casa[indiceDeCartas_Casa[casaActual]] != 0) {
            indiceDeCartas_Casa[casaActual]++;
        }
        */

        //auxIndex = indiceDeCartas_Casa[casaActual];
        cartasEnMazo_Casa[casaActual][indiceDeCartas_Casa[casaActual]] = pedirCarta();
        indiceDeCartas_Casa[casaActual]++;

        //auxIndex = indiceDeCartas_Casa[casaActual];
        cartasEnMazo_Casa[casaActual][indiceDeCartas_Casa[casaActual]] = pedirCarta();
        indiceDeCartas_Casa[casaActual]++;
    }

    //La casa de cada jugador revela una de sus cartas y oculta la otra
    for(uint8_t casaActual=0; casaActual<cantidadDeJugadores; casaActual++) {
        printf("\nCartas de la casa del jugador 1: ");
        auxIndex = indiceDeCartas_Casa[casaActual] - 2;
        mostarCarta(cartasEnMazo_Casa[auxIndex]);
    }
}

uint8_t pedirCarta(void){
    randomNumber(12);
}

uint8_t randomNumber(uint8_t modulo){
    //modulo
}

void mostrarCarta(uint8_t carta){
    if(carta>=2 && carta<=12){
        printf("%d", carta);
    } else if(carta=='A' || carta=='J' || carta=='Q' || carta=='K'){
        printf("%c", carta);
    }
}

void finDelJuego(void){

}

/*void input(unsigned char *var, unsigned char tipo){
    switch(tipo){
    case 'c':
        scanf("%c", &var);
        fflush(stdin);
        break;
    case 'd':
        scanf("%d", &var);
        fflush(stdin);
        break;
    default:
        printf("\nERROR AL INGRESAR DATO\n");
    }   return;
}*/
