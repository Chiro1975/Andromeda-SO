/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"


idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint32_t) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/

          
//DPL = 0.                                      
#define IDT_ENTRY(numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) CS_RING_0;                                                     \
    idt[numero].attr = (uint16_t) 0x8E00;                                                          \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);

//DPL = 3. 
#define IDT_ENTRY_3(numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) CS_RING_0;                                                     \
    idt[numero].attr = (uint16_t) 0xEE00;                                                          \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);


// No sabemos donde van a estar las isr de antemano.
// Por ende vamos a llamar a esta funcion cuando sepamos.
void idt_init() {
    // Excepciones

    // Define las interruption gates dentro de la IDT.
    // Completa las posiciones 0 a 21 que corresponden a las excepciones.
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);
    IDT_ENTRY(20);
    IDT_ENTRY(21);
    IDT_ENTRY(32);
    IDT_ENTRY(33);
    IDT_ENTRY_3(137);
    IDT_ENTRY_3(138);
    IDT_ENTRY_3(139);

    IDT_ENTRY(150);
    IDT_ENTRY_3(151);
 
}

uint8_t tieneError(uint8_t excepcion){
    if(excepcion == 8 || excepcion == 10 || excepcion == 11 || excepcion == 12 || excepcion == 13 || excepcion == 14 || excepcion == 17 || excepcion == 21){
        return 1;    
    }else{
        return 0;    
    }
}
