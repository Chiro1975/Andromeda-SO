; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"
%define GDT_IDX_8_CODIGO_SYSTEM 0x0008
%define GDT_IDX_10_DATA_SYSTEM  0x000A
%define GDT_IDX_12_SCREEN       0x000C
%define STACK_BASE 0x27000
%define TSS_INIT_INDX 0X000D  
%define TSS_IDLE_INDX 0x000E
global start

extern GDT_DESC ; Declaro el segmento de la GDT
extern IDT_DESC ; Declaro el segmento de la IDT
extern idt_init ; Declaro la funcion para inicializar la idt
extern pic_reset  ; Declaro la funcion que resetea el PIC
extern pic_enable ; Declaro la funcion para activar el PIC
extern mmu_init   ; Declaro la funcion para inicializar las paginas libres del kernel
extern mmu_initKernelDir ; Declaro la funcion para inicializar el mapeo del kernel
extern mmu_mapPage  ; Declaro la funcion para mapear
extern mmu_unmapPage;
extern tss_init ; Declaro la función que inicializa la TSS de la tarea idle.
extern sched_init
extern game_init
extern printDebug
extern handler137C
extern handler138C
extern handler139C

;; Saltear seccion de datos
jmp start

; ACA SOMOS  UNA MAQUINA ORGA1 . MEMORIA / INSTRUCCIONES.

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    

    ; Habilitar A20
    call A20_enable ; Habilito la linea A20 del procesador 
    call A20_check

    ; Cargar la GDT

    lgdt [GDT_DESC] ;Con esto, cargamos el puntero a la GDT que se encuentra en la variable GTD_DESC, 2 bytes mas adelante

    ; Setear el bit PE del registro CR0

    mov eax, cr0  ; Rescato el valor del registro CR0, esto lo hago por que CR0 no se puede modificar
    or  eax, 0x1  ; Cambio el primer bit a 1, para setear el modo protegido 
    mov cr0, eax  ; Vuelvo a cargar el registro en CR0, pero ahora modificado
   
    ; Saltar a modo protegido
    ; con este jmp modificamos el code segment.
    ; ahora estamos ejecutando codigo dentro del segmento de codigo.
    jmp (GDT_IDX_8_CODIGO_SYSTEM<<3):modoProtegido

modoProtegido:
BITS 32

    ; Establecer selectores de segmentos
    mov ax, GDT_IDX_10_DATA_SYSTEM
    shl ax, 3          ; Shifteo 3 para tener el selector de datos
    mov ds, ax         ; Muevo al registro DS el selectro de datos 
    mov es, ax         ; Repito lo mismo para es
    mov fs, ax         ; Repito lo mismo para fs
    mov gs, ax
    mov ss, ax

    ; Establecer la base de la pila

     mov ebp, STACK_BASE
     mov esp, STACK_BASE
    
    ; Imprimir mensaje de bienvenida

    ; Inicializar pantalla
    
    ; Muevo a ax el index del segmento de pantalla
    mov ax, GDT_IDX_12_SCREEN
    ; Shifteo 3 a la izquierda para obtener el selector
    shl ax, 3
    ; Muevo a un segmento de datos para manipular la pantalla
    mov fs, ax
    ; Inicializo el contador para la pantalla, aumenta de a dos
    xor bx, bx
    ;Muevo en cx el dato del pixel
    ;D = LIGHT MAGENTA
    ;9 = LIGHT BLUE
    ;0 = Caracter vacio
    mov cx, 0xD900
    ;1101 1001 0000 0000
    ;80*40*2 = 6400 bytes/2 = 3200 bytes
 	mov dx, 0x0000 ; uso el byte para pintar de negro
.primeraLineaNegro:

	mov [fs:bx], dx
	add bx, 2
	cmp bx, 160            ; Esto es solo para recorrer la primera fila 
	jne .primeraLineaNegro

.cicloPrinteo:
	

    mov [fs:bx], cx
    add bx, 2

    cmp bx, 6400           ; Aca solo pinto 80x39 
    jne .cicloPrinteo
    
    call idt_init
    
    lidt [IDT_DESC]
 
    ; Configurar controlador de interrupciones
   
    
    call pic_reset
    call pic_enable


    ; Inicializar el manejador de memoria
 
    ; Carga puntero de donde saca las paginas libres
    call mmu_init

    ; Inicializar el directorio de paginas

    call mmu_initKernelDir

    ; Muevo a cr3 en los 20 bits mas significativos de la direccion fisica de la PD (estan en 0 los primeros 12)
    mov cr3, eax
    
    ; Cargar directorio de paginas

    mov eax, cr0

    ; Habilitar paginacion

    ;1000 0000 0000 0000 0000 0000 0000 0000
    or eax, 0x80000000

    mov cr0, eax

;void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attrs) {
    
    push 2
    push 0x00400000
    push 0x0050E000
    mov eax, cr3
    push eax
    call mmu_mapPage
    add esp, 16
    mov BYTE [0x0050E027], 0x1

;void mmu_unmapPage(uint32_t cr3, uint32_t virtual) 
    push 0x0050E000
    mov eax, cr3
    push eax
    call mmu_unmapPage
    add esp, 8
    mov BYTE [0x0050E027], 0x1
    

    ; Inicializar tss
    
    ; Obtenemos el CR3 del kernel.
    mov eax, cr3
    
    ; Inicializar la tss idle.
    push eax
    call tss_init
    pop eax

    ; Inicializar el scheduler
    call sched_init
   
    ; Falta inicializar las tss de todas las tareas
    ; y llamar a la funcion para crearlas (????????
    ; es decir, tener una TSS valida en la gdt.
    call game_init

    ; Imprime las posiciones iniciales en pantalla.
    call printDebug

    ;xchg bx, bx
    ;Debugeamos el handler137C.
    ;mov eax, 0
    ;mov ecx, 0
    ;push 0 ; tarea 0
    ;push 1 ; sin morty
    ;push 1 ; cross
    ;push 20 ; y
    ;push 20 ; x
    ;call handler137C
    ;xchg bx, bx

    ; Debugeamos handler139C were is Morty.
    ;xchg bx, bx
    ;push 0 ; tarea 0
    ;call handler139C

    ;xor ecx, ecx
    ;xor edx, edx

    ; eax es un puntero a una estructura posMapa.
    ;mov cl, [eax] ;ecx
    ;mov dl, [eax+1] ;edx
    
        
    ; Cargar la tarea inicial.
    mov ax, TSS_INIT_INDX
    shl ax, 3 ; Estamos poniendo privilegios en 0.
    ltr ax

        
    ; Habilitar interrupciones    
    sti
 
    ; Saltar a la primera tarea : IDLE.
    jmp (TSS_IDLE_INDX << 3):0x0
    

    ; Inicializar tss de la tarea Idle


    ; Inicializar la IDT
    
    ; Cargar IDT
   
    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
