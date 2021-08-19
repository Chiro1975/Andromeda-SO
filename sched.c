/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"



void sched_init() {
    // Inicializo el codigo de la proxima tarea a ejecutar
    proximaTarea = 0;
    
    // Inicializo todas las tareas como vivas
    for(int i = 0; i < 25; i++){
        tasks[i].alive = 1;   
        tasks[i].x = 0;
        tasks[i].y = 0;
        tasks[i].type = 0; 
    }
}

// Devuelve el SELECTOR en la GDT de la TSS para la proxima tarea
// a ser ejecutada.

int16_t sched_nextTask() {


    if(congelado){
        return GDT_IDX_14_IDLE_TSS << 3;    
    }else if(gameOver){
        return GDT_IDX_14_IDLE_TSS << 3;     
    }

    if(proximaTarea == MAX_TAREAS){
        proximaTarea = 0;
        if(tasks[MAX_TAREAS].alive){
            return (BASE_GDT_TAREAS + MAX_TAREAS) << 3;
        }else{
            return GDT_IDX_14_IDLE_TSS << 3;        
        }
    }else{
        proximaTarea++;
        if(tasks[proximaTarea-1].alive){
            return (BASE_GDT_TAREAS + (proximaTarea - 1)) << 3;   
        }else{
            return GDT_IDX_14_IDLE_TSS << 3;             
        }
    }
}

void desalojarTarea(uint8_t tarea){
    tasks[tarea].alive = 0;
}



