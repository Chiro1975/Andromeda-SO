/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"
#include "tss.h"

typedef enum e_taskType {
    RickC137       = 1,
    MortyC137      = 2,
    RickD248       = 3,
    MortyD248      = 4,
    Cronenberg     = 5,
    CronenbergC137 = 6,
    CronenbergD248 = 7,
} e_taskType_t;

void game_init();
extern void game_init();

typedef struct posMapa {
    uint8_t x;
    uint8_t y;
}posMapa;

posMapa obtenerCoordenadas(uint32_t phy);
uint32_t obtenerFisica(posMapa coordenadas);

extern posMapa obtenerCoordenadas(uint32_t phy);
extern uint32_t obtenerFisica(posMapa coordenadas);

uint8_t portalGunUtilizada;

uint32_t CR3MortyC137;
uint32_t CR3MortyD248;

//TODO!!!!!!!!!!!!!!!!!
//Esto falta inicializarlo. OK.
uint8_t RickC137TareaMortyAsociada;
uint8_t RickD248TareaMortyAsociada;

// Declaro variable global para saber si Rick puede usar el servicio 137
void limpiarPortalGun();
extern void limpiarPortalGun();

uint32_t contadorRick1PortalGun;
uint32_t contadorRick2PortalGun;

void trasladarCodigo(uint8_t tarea, uint32_t fisicaSrc, uint32_t fisicaDestino);
void moveCharacter(uint32_t x, uint32_t y, uint8_t tarea, uint8_t cross);
uint8_t verTareaValida(uint32_t withMorty, uint8_t tarea);

void pintarPantalla(uint8_t tarea, posMapa coordenadasSrc, posMapa coordenadasDestino); 

/*Implementacion de sysCalls en C*/
uint32_t handler137C(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty, uint32_t tarea);
extern uint32_t handler137C(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty, uint32_t tarea);

void handler138C(uint32_t codigoUniverso, uint8_t tarea);
extern void handler138C(uint32_t codigoUniverso, uint8_t tarea);

void handler139C(int32_t *x, int32_t *y, uint8_t tarea);
extern void handler139C(int32_t *x, int32_t *y, uint8_t tarea);

// Variables globales para score.
uint8_t scoreRickC137;
uint8_t scoreRickD248;
// 1 si estoy en modo debug, 0 si no.
uint8_t debug;

// Game over
uint8_t gameOver;
uint8_t ganador;
uint8_t congelado;

uint8_t esCongelado();
uint8_t esDebug(); 

void congelarJuego();
void resumirJuego();

void debugearJuego();
void noDebugearJuego();

extern uint8_t esCongelado();
extern void congelarJuego();
extern void resumirJuego();
extern uint8_t esDebug(); 
extern void debugearJuego();
extern void noDebugearJuego();

uint8_t x_portal;
uint8_t y_portal;
uint8_t primerPortal;

#endif  /* !__GAME_H__ */
