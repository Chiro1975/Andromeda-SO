/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/*Color de la pantalla*/
#define SCREEN_BACKGROUND       0xD900 // Magenta
#define RICK137_COLOR           0x1 // Azul
#define RICK248_COLOR           0x04 // Rojo 
#define MORTY_COLOR             0x06  // Marron  
#define CRONENBERG_COLOR        0x08  // Gris

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  40 // X
#define SIZE_M                  80 // Y

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 46 //Arbitrario, en realidad tiene que ir 8196, pero como vamos a usar menos, no hace falta definirlo

#define GDT_IDX_NULL_DESC           0

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)
#define GDT_IDX_8_CODIGO_SYSTEM     8
#define GDT_IDX_9_CODIGO_USER 		9
#define GDT_IDX_10_DATA_SYSTEM		10
#define GDT_IDX_11_DATA_USER 		11
#define GDT_IDX_12_SCREEN 			12
#define GDT_IDX_13_INIT_TSS        13
#define GDT_IDX_14_IDLE_TSS        14


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

																								  //INDICE           TI RPL
#define CS_RING_0   				(GDT_IDX_8_CODIGO_SYSTEM  << 3)         // 0000 0000 0100 0000; 0 0000 0000 1000 0  00
#define CS_RING_3					((GDT_IDX_9_CODIGO_USER   << 3) | 0x3) // 0000 0000 0100 1011; 0 0000 0000 1001 0  11
#define DS_RING_0 					(GDT_IDX_10_DATA_SYSTEM   << 3)			// 0000 0000 0101 0000; 0 0000 0000 1010 0  00
#define DS_RING_3		 			((GDT_IDX_11_DATA_USER    << 3) | 0x3) // 0000 0000 0101 1011; 0 0000 0000 1011 0  11



/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */


#define IDLE_TASK_INIT_DIR        0x0001A000 // Dirección donde comienza a ejecutar la tarea Idle.
#define STACK_BASE_KERNEL         0x27000 // Dirección base para el stack del Kernel.
#define EFLAGS_DEFAULT_VALUE      0x00000202 // Bit 1 y 9 prendidos. 
#define TSS_IOMAP_ALL_PORTS_DISABLED 0xFFFF
/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR          0x00027000
#define KERNEL_PAGE_TABLE_0      0x00028000
#define CRYSTALS_MAP             0x00029000

#define TASK_CODE                0x08000000 /* direccion virtual del codigo */
#define TABLE_SIZE               1024
#define INICIO_PAGINA_LIBRE      0x00100000
#define PAGE_SIZE 				 0x00001000
#define TASK_SRC				 0x00010000
#define TAREA_FINAL              (0x2000-1) // Calcula la dirección base de la pila de la tarea.
#define BASE_CRONENBERG          0x400000

/* Scheduler */
#define BASE_GDT_TAREAS          15
#define MAX_TAREAS               24

#endif  /* !__DEFINES_H__ */
