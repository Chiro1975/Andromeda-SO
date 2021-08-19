/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "task.h"


// Macro para inicializar la TSS para la tarea i esima.
// i = 0 , 1 , ... , 24

tss tarea_0;
tss tarea_1;
tss tarea_2;
tss tarea_3;
tss tarea_4;
tss tarea_5;
tss tarea_6;
tss tarea_7;
tss tarea_8;
tss tarea_9;
tss tarea_10;
tss tarea_11;
tss tarea_12;
tss tarea_13;
tss tarea_14;
tss tarea_15;
tss tarea_16;
tss tarea_17;
tss tarea_18;
tss tarea_19;
tss tarea_20;
tss tarea_21;
tss tarea_22;
tss tarea_23;
tss tarea_24;


void game_init() {

    // Inicializamos el tipo de las tareas
    tasks[0].type = 1;
    tasks[1].type = 2;
    tasks[2].type = 3;
    tasks[3].type = 4;
    for(int i = 4; i < 25; i++){
        tasks[i].type = 5;  
    }


    // Creamos las tareas.
    newTask(0,(uint32_t) &tarea_0);
    newTask(1,(uint32_t) &tarea_1);
    newTask(2,(uint32_t) &tarea_2);
    newTask(3,(uint32_t) &tarea_3);
    newTask(4,(uint32_t) &tarea_4);
    newTask(5,(uint32_t) &tarea_5);
    newTask(6,(uint32_t) &tarea_6);
    newTask(7,(uint32_t) &tarea_7);
    newTask(8,(uint32_t) &tarea_8);
    newTask(9,(uint32_t) &tarea_9);
    newTask(10,(uint32_t) &tarea_10);
    newTask(11,(uint32_t) &tarea_11);
    newTask(12,(uint32_t) &tarea_12);
    newTask(13,(uint32_t) &tarea_13);
    newTask(14,(uint32_t) &tarea_14);
    newTask(15,(uint32_t) &tarea_15);
    newTask(16,(uint32_t) &tarea_16);
    newTask(17,(uint32_t) &tarea_17);
    newTask(18,(uint32_t) &tarea_18);
    newTask(19,(uint32_t) &tarea_19);
    newTask(20,(uint32_t) &tarea_20);
    newTask(21,(uint32_t) &tarea_21);
    newTask(22,(uint32_t) &tarea_22);
    newTask(23,(uint32_t) &tarea_23);
    newTask(24,(uint32_t) &tarea_24);

    //Inicizalizo variable para el modo debug
    debug = 0;
    congelado = 0;
    primerPortal = 0;

    // Inicializo variables para syscall usePortalGun.
    portalGunUtilizada = 0;
    contadorRick1PortalGun = 0;
    contadorRick2PortalGun = 0;

    // Las posiciones de las tareas ya estan inicializadas.
    
    // Inicializamos variables
    RickC137TareaMortyAsociada = 1;
    RickD248TareaMortyAsociada = 3;
    scoreRickC137 = 0;
    scoreRickD248 = 0;
    ganador = 0;

    actualizarPuntajes();
}

// Dada una pos. fisica en el mundo cronemberg,
// obtiene las coordenadas x e y.

// Revisar esta cuenta.
posMapa obtenerCoordenadas(uint32_t phy){

    // Primero calculo la cantidad de offset en celdas (2bytes cada una).
    uint32_t offset = (phy - BASE_CRONENBERG) / 0x2000;
    
    // TABLERO : (40x80) CELDAS.
    // Ahora calculamos el offset en celdas DENTRO de la fila.
    // Es decir, en que columna encuentro la celda.
    uint32_t offsetFila = offset % 80;

    // Ahora calculamos el offset en celdas DENTRO de la columna
    // Es decir, en que fila encuentro la celda.
    uint32_t offsetColumna = offset / 80;

    posMapa mapa;
    mapa.x = offsetColumna;
    mapa.y = offsetFila;
    return mapa;

}



//Dadas coordenadas x e y, obtiene la direccion fisica en mundo cronberg.
uint32_t obtenerFisica(posMapa coordenadas){
    return BASE_CRONENBERG + 80 * coordenadas.x * 0x2000 + coordenadas.y * 0x2000;
}

void limpiarPortalGun(){
    portalGunUtilizada = 0;
}

uint8_t verTareaValida(uint32_t withMorty, uint8_t tarea){
	  uint8_t taskType = tasks[tarea].type;
    if(taskType == 5 || taskType == 6 || taskType == 7){
        // es una tarea Cromberg
        return 0;    
    }else if(taskType == 1 || taskType == 3){
           // ES una tarea RICK. Hay que verificar que no se haya solicitado el servicio
           // en el mismo tick de reloj. 
        if(portalGunUtilizada){
            return 0;            
        }
    }else{
           // Es una tarea Morty. 
           if(taskType == 2 && (contadorRick1PortalGun < 10 ||  withMorty)){
                 return 0;
           }else if(taskType == 4 && (contadorRick2PortalGun < 10 || withMorty)){
                  return 0;           
            }    
    }
    return 1;
}


void trasladarCodigo(uint8_t tarea, uint32_t fisicaSrc, uint32_t fisicaDestino){

   
   uint32_t newTASK_CODE = TASK_CODE+0x2000;
	
   //Mapeo con el CR3 del kernel a la direccion fisica de la tarea en la posicion actual, para realizar el copiado a la nueva direccion
   mmu_mapPage(KERNEL_PAGE_DIR, TASK_CODE, fisicaSrc, 3);
   mmu_mapPage(KERNEL_PAGE_DIR, TASK_CODE+0x1000, fisicaSrc+0x1000, 3);

   //Mapeo dos paginas mas para la nueva posicion de la tarea
   mmu_mapPage(KERNEL_PAGE_DIR, newTASK_CODE, fisicaDestino, 3);
   mmu_mapPage(KERNEL_PAGE_DIR, newTASK_CODE+0x1000, fisicaDestino+0x1000, 3);
    
    uint32_t cr3_tarea;
    uint32_t cr3_actual = rcr3(); 
   // Debemos cambiar por el cr3 del kernel. 
   if(tasks[tarea].type == MortyC137){
        cr3_tarea = CR3MortyC137;

   } else if (tasks[tarea].type == MortyD248){
         cr3_tarea = CR3MortyD248;
   } else {
         cr3_tarea = rcr3();   
   }
 
   lcr3(KERNEL_PAGE_DIR);

   //Se copia
   uint32_t *src =  (uint32_t *) TASK_CODE;
   uint32_t *dest = (uint32_t *) newTASK_CODE;

   //2048
  
   for (int i=0; i<2048; i++){
   		dest[i] = src[i]; 
   }
   
   // Volvemos al cr3 viejo
   lcr3(cr3_actual);

   //Desmapeo las direcciones lineales del kernel
   mmu_unmapPage(KERNEL_PAGE_DIR, TASK_CODE);
   mmu_unmapPage(KERNEL_PAGE_DIR, TASK_CODE+0x1000);

   mmu_unmapPage(KERNEL_PAGE_DIR, newTASK_CODE);
   mmu_unmapPage(KERNEL_PAGE_DIR, newTASK_CODE+0x1000);

   //Desmapeo la posicion de la celda actual con el CR3 de la tarea
   mmu_unmapPage(cr3_tarea, TASK_CODE);
   mmu_unmapPage(cr3_tarea, TASK_CODE+0x1000);

   //Mapeo la nueva posicion con el codigo ya copiado
   mmu_mapPage(cr3_tarea, TASK_CODE, fisicaDestino, 7);
   mmu_mapPage(cr3_tarea, TASK_CODE+0x1000, fisicaDestino+0x1000, 7);
    
}

void moveCharacter(uint32_t x, uint32_t y, uint8_t tarea, uint8_t cross){
	portalGunUtilizada = 1;
    
	uint32_t x_new = ( tasks[tarea].x + x ) % 40;
    uint32_t y_new = ( tasks[tarea].y + y ) % 80;   
    

    posMapa coordenadasSrc;
    coordenadasSrc.x = tasks[tarea].x;
    coordenadasSrc.y = tasks[tarea].y;

    posMapa coordenadasDestino;
    coordenadasDestino.x = x_new;
    coordenadasDestino.y = y_new;

    // Calculamos las direcciones fisicas correspondientes
    uint32_t fisicaSrc     = obtenerFisica(coordenadasSrc); 
    uint32_t fisicaDestino = obtenerFisica(coordenadasDestino);   

    if(cross){
        trasladarCodigo((uint8_t) tarea, fisicaSrc, fisicaDestino);
        pintarPantalla((uint8_t) tarea, coordenadasSrc, coordenadasDestino); 
        // Actualizamos la posicion de la tarea
        tasks[tarea].x = x_new;
        tasks[tarea].y = y_new;

        // breakpoint(); hasta aca llega bien
    }else{ 
          // No cruzo el portal.
          // Tengo mapeado dicha memoria y puedo hacer lo que quier
          // con el codigo de lo que haya ahi.
          mmu_mapPage(rcr3(), TASK_CODE+0x2000, fisicaDestino, 7);
          mmu_mapPage(rcr3(), TASK_CODE+0x3000, fisicaDestino+0x1000, 7);  

          // Imprimimos el portal.
          imprimirPortal(x_new, y_new);
    }
    
    uint8_t taskType = tasks[tarea].type;
    switch(taskType){
    	case RickC137:
    		contadorRick1PortalGun++;
    		break;
    	case MortyC137:
    		contadorRick1PortalGun = contadorRick1PortalGun - 10;
    		break;
    	case RickD248:
    		contadorRick2PortalGun++;
    		break;
    	case MortyD248:
    		contadorRick2PortalGun = contadorRick2PortalGun - 10;
    		break;			
    }

}


void pintarPantalla(uint8_t tarea, posMapa coordenadasSrc, posMapa coordenadasDestino) {
    const char text[2] = {' ', 0};

    uint8_t taskType = tasks[tarea].type;
    uint8_t color;

    switch(taskType){
    	case RickC137:
    		color = RICK137_COLOR;
    		break;
    	case MortyC137:
    		color = MORTY_COLOR;
    		break;
    	case RickD248:
    		color = RICK248_COLOR;
    		break;
    	case MortyD248:
    		color = MORTY_COLOR;
    		break;	
    	case Cronenberg:
    		color = CRONENBERG_COLOR;  
    		break;				
    }

    // Primero despintamos la posicion src.
    uint16_t attrib = (C_FG_LIGHT_MAGENTA << 4) | (color & 0x0F);
    print((const char*)&text, coordenadasSrc.y, coordenadasSrc.x, attrib);
    
    // Pintamos la nueva posicion
     uint16_t attrib2 = (C_FG_BLACK << 4) | (color & 0x0F);
    print_dec(tarea, 1, coordenadasDestino.y, coordenadasDestino.x, attrib2);
}

uint32_t handler137C(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty, uint32_t tarea){
    // Primero verificamos si la tarea es valida.

    screen_printCronenbergTable();
    
	uint8_t valida = verTareaValida(withMorty, (uint8_t) tarea);
	uint8_t taskType = tasks[tarea].type;

	if (!valida){
		return 0;
	}
    // Calculamos la nueva posicion en pantalla donde se encontrará la tarea.
    
    // Si llegamos a esta instancia del codigo, sabemos que
    // se puede utilizar el servicio.
    if(cross){ // Pisa el codigo destino por el suyo.
    	if(withMorty){
    		//Muevo a Rick
    		moveCharacter(x, y, (uint8_t) tarea,1);
            
    		//Me fijo cual Morty le corresponde para moverlo
    		if (taskType == RickC137){
                
    			moveCharacter(x, y, (uint8_t) RickC137TareaMortyAsociada,1);
    		} else {	
             
    			moveCharacter(x, y, (uint8_t) RickD248TareaMortyAsociada,1);	
    			}
        } else {
       		//Muevo solo a un personaje
       		moveCharacter(x, y, (uint8_t) tarea,1);	
    		}

    } else { // No se pisa el codigo. Solo se mapea.
             // Imprimimos el portal
           moveCharacter(x, y, (uint8_t) tarea, 0);
    	}
    	return 1;

}

// Esta interrupcion se va a llamar desde el contexto
// de una tarea (tarea) que fue capturada por el universo codigoUniverso.
void handler138C(uint32_t codigoUniverso, uint8_t tarea){

    
    uint8_t taskType = tasks[tarea].type;

    // Si es una tarea Rick o Morty del mismo universo, no hago nada.
    if(codigoUniverso == 0xC137 && (taskType == RickC137 || taskType == MortyC137)){
        return;
    }else if(codigoUniverso == 0xD248 && (taskType == RickD248 || taskType == MortyD248)){
        return;    
    }else{

        
        // Estamos solicitando la interrupcion desde una tarea
        // que fue conquistada. 
        
        // el codigoUniverso nos dice desde que universo fue conquistada esta tarea.

        // Matamos la tarea.
        
        tasks[tarea].alive = 0;
        
        // Sumamos el contador correspondiente.
        // Si capturo todas las mentes, se acaba el juego.
        if(codigoUniverso == 0xC137){
            scoreRickC137++;
            if(scoreRickC137 == 21){
                ganador = RickC137;
                gameOver = 1;    
                imprimirPantallaFinal();        
            }else if(scoreRickC137 + scoreRickD248 >= 21){
                ganador = 5; // EL 5 se refiere a que no gana nadie.
                gameOver = 1;
                imprimirPantallaFinal();            
            }        
        }else{
            scoreRickD248++;        
            if(scoreRickD248 == 21){
                ganador = RickD248;
                gameOver = 1;      
                imprimirPantallaFinal();       
            }else if(scoreRickC137 + scoreRickD248 >= 21){
                ganador = 5; // EL 5 se refiere a que no gana nadie.
                gameOver = 1;
                imprimirPantallaFinal(); 
            }
        }

        //Si es una tarea rick o morty se termina el juego.
    
        // Si la tarea pisada es rick, gano morty.
        if(taskType == RickC137 || taskType == MortyC137){
            gameOver = 1;      
            ganador = RickD248;  
            imprimirPantallaFinal();     
        }else if(taskType == RickD248 || taskType == MortyD248){
            gameOver = 1;
            ganador = RickC137;      
            imprimirPantallaFinal();   
        }
    }

    actualizarPuntajes();
}


void handler139C(int32_t* x, int32_t* y, uint8_t tarea){
    // Este servicio solo puede ser utilizado por los Ricks
    // para encontrar a su morty.
    uint8_t taskType = tasks[tarea].type;

    uint8_t offset_x;
    uint8_t offset_y;

    if(taskType == MortyC137 || taskType == MortyD248 || taskType == Cronenberg){
    	*x = -1;
    	*y = -1;    
    }else{
        posMapa rickActual;
        posMapa mortyActual;
        rickActual.x = tasks[tarea].x;
        rickActual.y = tasks[tarea].y;
        if(taskType == RickC137){
            mortyActual.x = tasks[RickC137TareaMortyAsociada].x;
            mortyActual.y = tasks[RickC137TareaMortyAsociada].y;
        }else{
            mortyActual.x = tasks[RickD248TareaMortyAsociada].x;
            mortyActual.y = tasks[RickD248TareaMortyAsociada].y; 
        }

        // Calculamos cuantas celdas hay entre rick y morty.
        if(rickActual.x > mortyActual.x){
            offset_x = rickActual.x - mortyActual.x;        
        }else{
            offset_x = mortyActual.x - rickActual.x;        
        }

        if(rickActual.y > mortyActual.y){
            offset_y = rickActual.y - mortyActual.y;        
        }else{
            offset_y = mortyActual.y -rickActual.y;        
        }
    }    
    *x = offset_x;
    *y = offset_y;
}

void debugearJuego(){
    debug = 1;    
}

void noDebugearJuego(){
    debug = 0;    
}

uint8_t esDebug() {
    return debug;
}

uint8_t esCongelado(){
    return congelado;
}

void congelarJuego(){
    congelado = 1; 
}

void resumirJuego(){
    congelado = 0;    
}
















