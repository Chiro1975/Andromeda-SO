/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"
#include "defines.h"

// Defino el codigo de la proxima tarea a ejecutar
uint16_t proximaTarea;

// Tareas vivos
//uint8_t tareasVivas[25];

typedef struct task{
    uint8_t alive;
    uint8_t x;
    uint8_t y;
    uint8_t type;
} task;

task tasks[25];

void sched_init();
extern void sched_init();

int16_t sched_nextTask();

void desalojarTarea(uint8_t tarea);
extern void desalojarTarea(uint8_t tarea);

#endif	/* !__SCHED_H__ */
