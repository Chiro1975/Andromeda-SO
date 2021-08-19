/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


unsigned int PROXIMA_PAGINA_LIBRE_KERNEL;

void mmu_init() {

	PROXIMA_PAGINA_LIBRE_KERNEL = INICIO_PAGINA_LIBRE;

}

//Devuelve un puntero a la pagina pedida e incrementa el puntero a la siguiente pagina libre
uint32_t mmu_nextFreeKernelPage() {
	//Asigno la pagina actual
	unsigned int PAGINA_LIBRE = PROXIMA_PAGINA_LIBRE_KERNEL;

	//Incremento el puntero a la siguiente pagina libre
	PROXIMA_PAGINA_LIBRE_KERNEL += PAGE_SIZE;
	return (uint32_t) PAGINA_LIBRE;
}

void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attrs) {

		//Asigno la entrada de la PD, quedandome con los 10 mas significativos.
		uint16_t PDIndex = virtual >> 22;

		//Asigno la entrada de la PT, quedanome con los 10 bits del medio.
		uint16_t PTIndex = (virtual << 10) >> 22; 

	   page_directory_entry *PDE = (page_directory_entry *) cr3;

	   if (PDE[PDIndex].present == 0){
	   		page_table_entry *newPT = (page_table_entry *) mmu_nextFreeKernelPage();
	   		for (int i=0; i<TABLE_SIZE; i++){
	   			newPT[i] = (page_table_entry) {0};
	   		}

	   		PDE[PDIndex].present = 1;
	   		PDE[PDIndex].user_supervisor = (uint8_t)  ((attrs & 0x2)>>1);
	   		PDE[PDIndex].read_write      = (uint8_t)  ((attrs & 0x4)>>2);
	   		PDE[PDIndex].page_table_base = ((uint32_t) newPT) >> 12; 
	   }

	 page_table_entry *PT = (page_table_entry *) ((PDE[PDIndex].page_table_base)<<12);
	 PT[PTIndex].present               = 1;
	 PT[PTIndex].user_supervisor       = (uint8_t)  ((attrs & 0x2)>>1);
	 PT[PTIndex].read_write            = (uint8_t)  ((attrs & 0x4)>>2);
	 PT[PTIndex].physical_address_base = (uint32_t) (phy >> 12);

	 tlbflush();

}

void mmu_unmapPage(uint32_t cr3, uint32_t virtual) {
	
	uint16_t PDIndex = virtual >> 22;

	uint16_t PTIndex = (virtual << 10) >> 22; 

	page_directory_entry *PDE = (page_directory_entry *) cr3;

	page_table_entry *PT = (page_table_entry *) ((PDE[PDIndex].page_table_base)<<12);

	PT[PTIndex] = (page_table_entry) {0};
	
}

uint32_t mmu_initKernelDir() {
   page_directory_entry *PDKernel   = (page_directory_entry *) KERNEL_PAGE_DIR;
   page_table_entry	    *PTKernel_0 = (page_table_entry *)     KERNEL_PAGE_TABLE_0;

   //Inicializamos todas la estrucutra en 0, tanto la PT como la PD
   for(int i = 0; i < TABLE_SIZE; ++i){
   	  PDKernel[i] = (page_directory_entry) {0};
   	  PTKernel_0[i]  = (page_table_entry)  {0};
   }

   //Completamos la primera entrada de la page directory
   PDKernel[0].present = 1;
   PDKernel[0].user_supervisor = 0;
   PDKernel[0].read_write      = 1; 
   PDKernel[0].page_table_base = ((uint32_t) PTKernel_0) >> 12;

   
   //Completamos toda la primera page table mapeando con identity mapping
   for (int i=0; i<TABLE_SIZE; i++){
   		PTKernel_0[i].present               = 1;
   		PTKernel_0[i].user_supervisor       = 0;
   		PTKernel_0[i].read_write            = 1;
   		PTKernel_0[i].physical_address_base = i;

   }

   return (uint32_t)PDKernel;
}

uint32_t mmu_initTaskDir(uint8_t tarea, uint32_t dst) {
    uint32_t codigoTarea;
    // Aca comienza el codigo de la tarea
    if(tarea >= 4){
       codigoTarea = TASK_SRC + 0x4*0x2000;     
    }else{
      if (tarea == 0){
       codigoTarea = TASK_SRC;        
      } else if (tarea == 1){
          codigoTarea = TASK_SRC + 0x2000;
      } else if (tarea == 2) {
         codigoTarea = TASK_SRC + 0x4000;
      } else if (tarea == 3){
         codigoTarea = TASK_SRC + 0x6000;
      }
    }
	

	page_directory_entry *PDE = (page_directory_entry *) mmu_nextFreeKernelPage();
	page_table_entry	 *PTE = (page_table_entry *)     mmu_nextFreeKernelPage();

	for(int i = 0; i < TABLE_SIZE; ++i){
   	  PDE[i] = (page_directory_entry) {0};
   	  PTE[i] = (page_table_entry)     {0};
   }

   //Completamos la primera entrada de la page directory
   PDE[0].present         = 1;
   PDE[0].user_supervisor = 1;
   PDE[0].read_write      = 1; 
   PDE[0].page_table_base = ((uint32_t) PTE) >> 12;

   
   //Completamos toda la primera page table mapeando con identity mapping
   for (int i=0; i<TABLE_SIZE; i++){
   		PTE[i].present               = 1;
   		PTE[i].user_supervisor       = 1;
   		PTE[i].read_write            = 1;
   		PTE[i].physical_address_base = i;

   } 
   
   //Queremos mapearle la direccion de memoria virtual TASK_CODE.
   //Es la que va a usar para acceder a su codigo.
   //uint32_t PDIndex = ((uint32_t) TASK_CODE) >> 22;
   //uint32_t PTIndex = (((uint32_t) TASK_CODE) << 10) >> 22; 

   //page_directory_entry *PDKernel = (page_directory_entry *) rcr3();

   //page_table_entry *PT = (page_table_entry *) ((PDKernel[PDIndex].page_table_base)<<12);


   // Mapeo al kernel la direccion TASK CODE.
   mmu_mapPage(KERNEL_PAGE_DIR, TASK_CODE, dst, 3);
   mmu_mapPage(KERNEL_PAGE_DIR, TASK_CODE+0x1000, dst+0x1000, 3);
    
 
   uint32_t *src =  (uint32_t *) codigoTarea;
   uint32_t *dest = (uint32_t *) TASK_CODE;

   //2048
   for (int i=0; i<2048; i++){
   		dest[i] = src[i]; 
   }

   mmu_unmapPage(KERNEL_PAGE_DIR, TASK_CODE);
   mmu_unmapPage(KERNEL_PAGE_DIR, TASK_CODE+0x1000);

   mmu_mapPage((uint32_t)PDE, TASK_CODE, dst, 7);
   mmu_mapPage((uint32_t)PDE, TASK_CODE+0x1000, dst+0x1000, 7);

   return (uint32_t) PDE;

}






