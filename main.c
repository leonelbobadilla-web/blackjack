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
uint16_t seed; //Variable para generación de números aleatorios
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
void mostrarCarta(uint8_t);
void finDelJuego(void);
uint8_t randomNumber(uint8_t);

//main
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

    //Reinicia el registro del mazo principal
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
        printf("\n\tPrimera carta de la casa del jugador %d: ", casaActual+1);
        auxIndex = indiceDeCartas_Casa[casaActual] - 2;
        mostrarCarta(cartasEnMazo_Casa[casaActual][auxIndex]);
    }

    //Se le asignan dos cartas a cada jugador
    for(uint8_t jugadorActual=0; jugadorActual<cantidadDeJugadores; jugadorActual++) {
        printf("\n\tPrimera carta de la casa del jugador %d: ", jugadorActual+1);
        auxIndex = indiceDeCartas_Casa[jugadorActual] - 2;
        mostrarCarta(cartasEnMazo_Casa[jugadorActual][auxIndex]);
    }
}

uint8_t pedirCarta(void){
    uint8_t carta = randomNumber(15);
    carta++;

    if(carta>=2 && carta<=12){
        return carta;
    } else if(carta==1){
        return 'A';
    } else if(carta==13){
        return 'J';
    } else if(carta==14){
        return 'J';
    } else if(carta==15){
        return 'J';
    }
}

uint8_t randomNumber(uint8_t modulo){
    seed ^= (seed << 7);
    seed ^= (seed >> 9);
    seed ^= (seed << 8);
    return (seed % modulo);
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
