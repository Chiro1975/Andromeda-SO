/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;

// Arreglo para almacenar los k aleatorios.
// las tareas las vamos a ubicar en multiplos de paginas de 8k.
/*uint32_t randomNumber[25] = {7191,76,6240,7876,5603,3165,
2185,1159,5408,7025,4375,1356,4472,7058,5718,5813,5356,6705,799,7695,
990,3674,5088,3982,2516};
*/

//uint32_t randomNumber[25] = {10,90,30,197,4,180,65,41,79,158,169,147,
//120,100,105,106,107,108,109,110,111,112,113,114,115};

uint8_t randomNumber_x[25] = {5,20,30,40,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

//uint8_t randomNumber_y[25] = {0, 10, 15,  20, 25, 30, 35, 40,  0,  4,  2, 16, 15, 24, 24, 18, 23, 38, 12, 25, 14, 22,  0, 28,74};

uint8_t randomNumber_y[25] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,71,72,73,74,75,75,75,75,75};



void tss_init(uint32_t cr3) {


    // 1) Completar los campos para la TSS de la tarea IDLE.
    // Llamar a la funcion con el CR3 del kernel.

    tss_idle.ss = DS_RING_0; // Definimos el segmento de pila como el del kernel.
    tss_idle.ds = DS_RING_0; // Definimos el segmento de pila como el del kernel.
    tss_idle.es = DS_RING_0;
    tss_idle.fs = DS_RING_0;
    tss_idle.gs = DS_RING_0;
    tss_idle.cs = CS_RING_0;
    tss_idle.cr3 = cr3;
    tss_idle.eip = IDLE_TASK_INIT_DIR;
    tss_idle.ebp = STACK_BASE_KERNEL;
    tss_idle.esp = STACK_BASE_KERNEL;
    tss_idle.eflags = EFLAGS_DEFAULT_VALUE;
    tss_idle.iomap = TSS_IOMAP_ALL_PORTS_DISABLED;
    
    gdt[GDT_IDX_14_IDLE_TSS].base_0_15 = (uint16_t) (uint32_t) &tss_idle;
    gdt[GDT_IDX_14_IDLE_TSS].base_23_16 = (uint8_t) (uint32_t) (&tss_idle) >> 16 ;
    gdt[GDT_IDX_14_IDLE_TSS].base_31_24 = (uint8_t) (uint32_t) (&tss_idle) >> 24 ;
}

// Devuelve el selector de la GDT a la TSS de la tarea creada.
void newTask(uint8_t tarea, uint32_t tss_addr){
   
    //uint32_t codigoTarea = TASK_SRC + tarea*0x2000; // Direccion en memoria donde inicia el codigo de la tarea. son dos paginas de 4k cu.
    
    // Debemos inicializar en la GDT una entrada que apunte a un descriptor de TSS. 
    // Debemos inicializar la TSS para la tarea.
    
    // Defino el indice en la GDT.
    uint16_t indice = 15 + tarea;
    
    
    // Declaramos el descriptor de la TSS.
    gdt[indice].limit_0_15 = sizeof(tss)-1;
    gdt[indice].limit_16_19 = 0x0;
    gdt[indice].base_0_15 = (uint16_t) tss_addr;
    gdt[indice].base_23_16 = (uint8_t) (tss_addr >> 16);
    gdt[indice].base_31_24 = (uint8_t) (tss_addr >> 24);
    gdt[indice].type = 0x9; // Indica una tarea inactiva.
    gdt[indice].s = 0;
    gdt[indice].dpl = 0x0; // Solo el kernel puede acceder al descriptor.
    gdt[indice].p = 1;
    gdt[indice].avl = 0x0;
    gdt[indice].l = 0x0;
    gdt[indice].db = 0x0;
    gdt[indice].g = 0x0;
    
    // Completamos los campos de la TSS.
    // Primero debemos pedirle dos páginas al kernel
    // para el mecanismo de paginación. (PD - PT).
    

    // Pedimos una dirección fisica.
    //uint32_t phy = BASE_CRONENBERG + randomNumber[tarea] * 0x2000;

    uint8_t phyDest_x = randomNumber_x[tarea];
    uint8_t phyDest_y = randomNumber_y[tarea];

    posMapa phyDest;
    phyDest.x = phyDest_x;
    phyDest.y = phyDest_y;
    
    uint32_t phy = obtenerFisica(phyDest);
    
    // Realizamos la conversión de dirección fisica
    // en mundo Cronebrg a posicion (x,y).
    posMapa coordenadas = obtenerCoordenadas(phy);

    // Almacenamos en el struct.
    tasks[tarea].x = coordenadas.x;
    tasks[tarea].y = coordenadas.y;


    // Tenemos que chequear la funcion mmu init task dir    

    // Pedimos un cr3 para paginacion.
    uint32_t cr3 = mmu_initTaskDir(tarea, phy);

    
    // Pedimos pagina para la pila nivel 0
    uint32_t stackLevel0Addr = mmu_nextFreeKernelPage();

    tss* tss_newTask = (tss*) (uint32_t*) tss_addr;
    
    tss_newTask->ss = DS_RING_3; // Definimos el segmento de pila como el del kernel.
    tss_newTask->ds = DS_RING_3; // Definimos el segmento de pila como el del kernel.
    tss_newTask->es = DS_RING_3;
    tss_newTask->fs = DS_RING_3;
    tss_newTask->gs = DS_RING_3;
    tss_newTask->cs = CS_RING_3;
    
    tss_newTask->cr3 = cr3;
    tss_newTask->eip = TASK_CODE;
    tss_newTask->ebp = TASK_CODE + TAREA_FINAL ;
    tss_newTask->esp = TASK_CODE + TAREA_FINAL ;
    tss_newTask->eflags = EFLAGS_DEFAULT_VALUE;
    tss_newTask->iomap = TSS_IOMAP_ALL_PORTS_DISABLED;   
    tss_newTask->esp0 = stackLevel0Addr + (0x1000 - 1);
    tss_newTask->ss0 = DS_RING_0;

    //return (indice << 3);

    if (tasks[tarea].type == MortyC137){
        CR3MortyC137 = cr3;
    } else if(tasks[tarea].type == MortyD248) {
        CR3MortyD248 = cr3;
    }
}
