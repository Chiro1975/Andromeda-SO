/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

uint8_t scancode[10] = {0x45, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46};

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
    for (c = cInit; c < cInit+cSize; c++) {
          p[f][c].c = character;
          p[f][c].a = attr;
    }}
}


//FALTA DEFINIR EN LOS DEFINES LOS COLORES
void print_excepcion(uint32_t valor_excepcion, uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi, uint32_t ebp, uint32_t ds, uint32_t es, uint32_t fs, uint32_t gs, uint32_t cr0, uint32_t cr1, uint32_t cr2, uint32_t cr3, uint32_t cr4, int32_t errorCode, uint32_t eip,uint32_t cs,uint32_t eflags,uint32_t esp,uint32_t ss){
    
    if (debug){
        
    char * nombre_excepcion = "?? - Desconocida";
    int cant_excepciones = 21;
    char * excepciones[] = {"0x0 - Divide-by-zero Error", "0x1 - Debug", "0x2 - Non-maskable Interrupt", "0x3 - Breakpoint", "0x4 - Overflow", "0x5 - Bound Range Exceeded", "0x6 - Invalid Opcode", "0x7 - Device Not Available", "0x8 - Double Fault", "0x9 - Coprocessor Segment Overrun", "0xA - Invalid TSS", "0xB - Segment Not Present", "0xC - Stack-Segment Fault", "0xD - General Protection Fault", "0xE - Page Fault", "0xF - (Reserved)", "0x10 - x87 Floating-Point Exception", "0x11 - Alignment Check", "0x12 - Machine Check", "0x13 - SIMD Floating-Point Exception", "0x14 - Virtualization Exception"};

    if (valor_excepcion < cant_excepciones) nombre_excepcion = excepciones[valor_excepcion];

    screen_drawBox(0, 5, 36, 36, 0, C_BG_BLACK);
    screen_drawBox(1, 6, 34, 34, 0, C_BG_LIGHT_GREY);
    print(nombre_excepcion, 6, 0, C_FG_RED | C_BG_BLACK);

    print("eax:", 7, 2, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(eax, 8, 12, 2, C_FG_RED | C_BG_LIGHT_GREY);

    print("ebx:", 7, 4, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ebx, 8, 12, 4, C_FG_RED | C_BG_LIGHT_GREY);

    print("ecx:", 7, 6, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ecx, 8, 12, 6, C_FG_RED | C_BG_LIGHT_GREY);

    print("edx:", 7, 8, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(edx, 8, 12, 8, C_FG_RED | C_BG_LIGHT_GREY);

    print("esi:", 7, 10, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(esi, 8, 12, 10, C_FG_RED | C_BG_LIGHT_GREY);

    print("edi:", 7, 12, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(edi, 8, 12, 12, C_FG_RED | C_BG_LIGHT_GREY);

    print("ebp:", 7, 14, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ebp, 8, 12, 14, C_FG_RED | C_BG_LIGHT_GREY);

    print("esp:", 7, 16, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(esp, 8, 12, 16, C_FG_RED | C_BG_LIGHT_GREY);

    print("eip:", 7, 18, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(eip, 8, 12, 18, C_FG_RED | C_BG_LIGHT_GREY);

    print("cs:", 7, 20, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cs, 4, 12, 20, C_FG_RED | C_BG_LIGHT_GREY);

    print("ds:", 7, 22, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ds, 4, 12, 22, C_FG_RED | C_BG_LIGHT_GREY);

    print("es:", 7, 24, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(es, 4, 12, 24, C_FG_RED | C_BG_LIGHT_GREY);

    print("fs:", 7, 26, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(fs, 4, 12, 26, C_FG_RED | C_BG_LIGHT_GREY);

    print("gs:", 7, 28, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(gs, 4, 12, 28, C_FG_RED | C_BG_LIGHT_GREY);

    print("ss:", 7, 30, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ss, 4, 12, 30, C_FG_RED | C_BG_LIGHT_GREY);

    print("eflags:", 7, 32, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(eflags, 8, 15, 32, C_FG_RED | C_BG_LIGHT_GREY);

    // derecha
    print("cr0:",22, 7, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr0, 8, 27, 7, C_FG_RED | C_BG_LIGHT_GREY);

    print("cr2:", 22, 9, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr2, 8, 27, 9, C_FG_RED | C_BG_LIGHT_GREY);

    print("cr3:", 22, 11, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr3, 8, 27, 11, C_FG_RED | C_BG_LIGHT_GREY);

    print("cr4:", 22, 13, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr4, 8, 27, 13, C_FG_RED | C_BG_LIGHT_GREY);
    //breakpoint();
    // printeamos stack
    uint32_t * espPrint = (uint32_t*) esp;
    print("stack", 22, 27, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(*espPrint, 8, 22, 30, C_FG_RED | C_BG_LIGHT_GREY);
    //print_hex(*(espPrint+0x20), 8, 22, 31, C_FG_RED | C_BG_LIGHT_GREY);
   // print_hex(*(espPrint+0x40), 8, 22, 32, C_FG_RED | C_BG_LIGHT_GREY);
   // print_hex(*(espPrint+0x60), 8, 22, 33, C_FG_RED | C_BG_LIGHT_GREY);
    
    
    }

}

void imprimirNumeroEsquinaSuperiorDerecha(uint8_t tecla){
    unsigned char forecolour = 12;
    unsigned char backcolour = 0;
    uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
        if ( tecla >= 2 && tecla <= 11) {
            if (tecla == 11){
                print_dec(0, 1,  79, 0, attrib);
            }
            print_dec(tecla-1, 1,  79, 0, attrib);
        }
    }

//void print_dec(uint32_t numero, uint32_t size,
 //uint32_t x, uint32_t y, uint16_t attr);

void printDebug(){

    
    for(int i = 0; i < 25; i++){

    uint8_t taskType = tasks[i].type;
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
        if(tasks[i].alive){
            uint16_t attrib = (SCREEN_BACKGROUND << 4) | (color & 0x0F);
            print_dec(i, 1, tasks[i].y, tasks[i].x, attrib); 
        }
    }

  
}

void actualizarPuntajes(){
    print_dec(scoreRickC137, 10, 6, 44, C_FG_LIGHT_MAGENTA);
    print_dec(scoreRickD248, 10, 62, 44, C_FG_LIGHT_MAGENTA);
}

void imprimirPantallaFinal(){

    screen_drawBox(10, 20, 20, 40, 0, C_FG_DARK_GREY);
    print("El ganador es:", 25, 15, C_FG_DARK_GREY );
    if(ganador == RickC137){
        print("RICK C137", 35, 20, C_FG_DARK_GREY );
    }else if(ganador == RickD248){
        print("RICK D248", 35, 20, C_FG_DARK_GREY );
    }else{
        print("NO HAY GANADOR", 30, 20, C_FG_DARK_GREY );
    }
}

void pantallaMagenta(){
    screen_drawBox( 0, 0, 40, 80, 0, 0xD9 );
    printDebug();
}
     
void imprimirPortal(uint32_t x_new, uint32_t y_new){
    if(primerPortal == 0){
       print(" ", y_new, x_new, C_BG_GREEN);  
       primerPortal = 1;
       x_portal = x_new;
       y_portal = y_new;
    }else{
       print(" ", y_new, x_new, C_BG_GREEN);  
       print(" ", x_portal, y_portal, 0xD9);  
       // Actualizo las coordenadas donde se ubica el nuevo portal.
       x_portal = x_new;
       y_portal = y_new;  
       
    }
      
}

void screen_printCronenbergTable(){
    uint8_t row = 0;
    for(int i = 4; i < 25; i++){
       print_dec(i,2,row,42,C_BG_CYAN);
       if(tasks[i].alive){
            if(tasks[i].type == CronenbergC137){
                print("B", row, 44, C_BG_BLUE);        
            }else if(tasks[i].type == CronenbergD248){
                print("R", row, 44, C_BG_RED );            
            }else{
                print("C", row, 44, C_BG_BLACK);                
            }        
        }else{
        print("X", row, 44, C_BG_BLACK);
        } 
        row += 4;
    }

    row = 0;
    for(int i = 4; i < 25; i++){
       print_dec(i,2,row,42,C_BG_CYAN);
       if(tasks[i].alive){
            if(tasks[i].type == CronenbergC137){
                print("B", row, 44, C_BG_BLUE);        
            }else if(tasks[i].type == CronenbergD248){
                print("R", row, 44, C_BG_RED );            
            }else{
                print("C", row, 44, C_BG_BLACK);                
            }        
        }else{
        print("X", row, 44, C_BG_BLACK);
        } 
        row += 4;
    }
    

}

