/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"


gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
//LIMITE:85C9

    [GDT_IDX_8_CODIGO_SYSTEM] = (gdt_entry) {
        .base_0_15   = 0x0000,
        .base_23_16  = 0x0000,
        .base_31_24  = 0x0000,
        .limit_0_15  = 0x85C9,  // 137 MEGAS lo pasamos a Kilobytes, que quedan en 137000 Kb, eso lo dividimos por 4 que queda 34250, le restamos uno y queda 0x85C9;
        .limit_16_19 = 0x0000,
        .type = 0x0A,             // 10 : EXECUTE/READ
        .s    = 0x1,              // 0 : Codigo/Datos.
        .dpl  = 0x0,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x1,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x1,              // Granularidad a bytes.
    },

    [GDT_IDX_9_CODIGO_USER] = (gdt_entry){
        .base_0_15   = 0x0000,
        .base_23_16  = 0x0000,
        .base_31_24  = 0x0000,
        .limit_0_15  = 0x85C9,  // 137 MEGAS lo pasamos a Kilobytes, que quedan en 137000 Kb, eso lo dividimos por 4 que queda 34250, le restamos uno y queda 0x85C9;
        .limit_16_19 = 0x0000,
        .type = 0xA,             // 10 : EXECUTE/READ
        .s    = 0x1,              // 0 : Codigo/Datos.
        .dpl  = 0x3,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x1,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x1,              // Granularidad a bytes.
    },

    [GDT_IDX_10_DATA_SYSTEM] = (gdt_entry) {
        .base_0_15   = 0x0000,
        .base_23_16  = 0x0000,
        .base_31_24  = 0x0000,
        .limit_0_15  = 0x85C9,  // 137 MEGAS lo pasamos a Kilobytes, que quedan en 137000 Kb, eso lo dividimos por 4 que queda 34250, le restamos uno y queda 0x85C9;
        .limit_16_19 = 0x0000,
        .type = 0x2,              // 2 : READ/WRITE
        .s    = 0x1,              // 1 : Codigo/Datos.
        .dpl  = 0x0,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x1,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x1,              // Granularidad a bytes.
    },

    [GDT_IDX_11_DATA_USER] = (gdt_entry) {
        .base_0_15   = 0x0000,
        .base_23_16  = 0x0000,
        .base_31_24  = 0x0000,
        .limit_0_15  = 0x85C9,  // 137 MEGAS lo pasamos a Kilobytes, que quedan en 137000 Kb, eso lo dividimos por 4 que queda 34250, tambien hay que restarle 4 kb, le restamos uno y queda 0x85C9;
        .limit_16_19 = 0x0000,
        .type = 0x2,              // 2 : READ/WRITE
        .s    = 0x1,              // 1 : Codigo/Datos.
        .dpl  = 0x3,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x1,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x1,              // Granularidad a bytes.
    },

    [GDT_IDX_12_SCREEN] = (gdt_entry) {
        .base_0_15   = 0x8000,
        .base_23_16  = 0x000B,
        .base_31_24  = 0x0000,
        .limit_0_15  = 0x1F3F,    //80*50*2 = 8000 Bytes - 1 = 7999 Bytes = 0x1F3F
        .limit_16_19 = 0x0000,
        .type = 0x2,              // 2 : READ/WRITE
        .s    = 0x1,              // 1 : Codigo/Datos.
        .dpl  = 0x0,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x1,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x0,              // Granularidad a bytes.
    },

    [GDT_IDX_13_INIT_TSS] = (gdt_entry) {

        .base_0_15   = 0x0000,
        .base_23_16  = 0x0000,
        .base_31_24  = 0x0000,
        .limit_0_15  = sizeof(tss)-1,    
        .limit_16_19 = 0x0000,
        .type = 0x9,              // 1001 : Indica una tarea inactiva (Busy = 0)
        .s    = 0x0,              // 0 : System
        .dpl  = 0x0,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x0,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x0,              // Granularidad a bytes.

    },
    
    [GDT_IDX_14_IDLE_TSS] = (gdt_entry) {
        .base_0_15   = 0x0000,
        .base_23_16  = 0x0000,
        .base_31_24  = 0x0000,
        .limit_0_15  = sizeof(tss)-1,    
        .limit_16_19 = 0x0000,
        .type = 0x9,              // 2 : READ/WRITE
        .s    = 0x0,              // 1 : Codigo/Datos.
        .dpl  = 0x0,              // El permiso es de Sistema
        .p    = 0x1,              // El bit de presente activado por estar en la memoria
        .avl  = 0x0,              // Nada util
        .l    = 0x0,              // 0 : Nos dice que nuestro segmento tiene tamaño de operacion 64 bits
        .db   = 0x0,              // 1 : Segmento tiene tam de operación de 32 bits.
        .g    = 0x0,              // Granularidad a bytes.
    }

};



gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};

