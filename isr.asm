; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:     dd 0xFFFF
sched_task_selector:   dw 0xFFFFFFFF
%define TSS_INIT_INDX 0X000D  
%define TSS_IDLE_INDX 0x000E

x:   dd 0xFFFFFFFF
y:   dd 0xFFFFFFFF

; Sirve como posicion de memoria backup para almacenar temporalmente el valor de eax.
eax_backup:            dd 0x00000000 

;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask

; Print excepcion.
extern print_excepcion

extern imprimirNumeroEsquinaSuperiorDerecha

extern handler137C
extern limpiarPortalGun

extern desalojarTarea

extern handler138C
extern handler139C

extern debugearJuego
extern noDebugearJuego
extern esCongelado
extern resumirJuego
extern congelarJuego
extern esDebug

; Saber si una excepcion provoca un error.
extern tieneError

extern pantallaMagenta;

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:

    ; Cualquier excepción (menos reloj, teclado ...)  

    mov [eax_backup], eax
    
    ; Verificamos si estamos en Modo Debug.
    call esDebug
   
    cmp eax, 0
    je .fin

    ; Estamos en modo debug.
    ; Primero congelamos el juego.
    
    call congelarJuego    

    
    ; Preservo eax.
    ; Piso eax con el numero de error.
    mov eax, %1

    ; Pasaje de parametros por pila a funcion imprimir pantalla debug.
    push eax
    call tieneError

    ; Hacemos que esp apunte al error code. (Pushie eax antes).
    add esp, 4 
    
    ; eax vale 1 si la excepcion tiene error code.
     cmp eax, 1
     je .handlerConError
   

    ; Si es handler sin error, colocamos un -1 para simular que hubo error no valido.
     push -1
    
    
.handlerConError:
    ; Pusheamos los parametros en el caso donde hay codigo de error.
    
    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax

    push gs
    push fs
    push es
    push ds
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push DWORD [eax_backup]
    push %1
    call print_excepcion

   
    ; Si tiene error code, liberamos 17 * 4 bytes.
    ; Si no tiene error code, liberamos 18 * 4 bytes.
    push %1
    call tieneError
    add esp, 4
    cmp eax, 1
    je .popConError
    
    ; Pop sin error
    add esp, 18*4
    jmp .fin    

.popConError:
    add esp, 17*4
    
.fin:
    ; Obtiene el selector de la tarea.
    ; Shifteo 3 a la derecha para obtener indice GDT
    ; Resto 15 para obtener el codigo de la tarea

    xor eax, eax
    str ax
    shr ax, 3
    sub ax, 15
    
    push eax

    ; Mato la tarea
    call desalojarTarea

    add esp, 4

    ; Salto a la tarea IDLE.   
    jmp (TSS_IDLE_INDX << 3):0x0    

    ; En caso de que algo salga mal
    jmp $

%endmacro

global _isr32
global _isr33
global _isr137
global _isr138
global _isr139

; Handler del clock
_isr32:
    
    pushad
   
    call nextClock
    call pic_finish1

    call esCongelado
    cmp eax, 1
    je .fin

    ; Resetamos la variable global para que Rick pueda volver a solicitar el servicio 137
    call limpiarPortalGun

    ; Busca proxima tarea a ejecutar
    call sched_nextTask

    ; Si el proximo es cero no salto.
    ;cmp ax, 0
    ;je .fin

    ; Verificar que la prox tarea no sea la actual
    str bx
    cmp ax, bx
    je .fin
    
    
    mov dx, ax
    shr dx, 3
  
    ; Realizo el salto
    mov word [sched_task_selector], ax
    jmp far [sched_task_offset]

.fin:
    popad
    iret

_isr33:
   
    pushad
    xor eax, eax
    in  al, 0x60

    ; Comparamos si la tecla presionada es 'y'.
    cmp al, 0x15 
    jne .fin 
    
    ; Verifico si el juego esta en modo Debug 
    call esDebug
    cmp eax, 0
    je .activarDebug

    ; Si llego acá modo debug esta ACTIVADO.
    ; Si el juego esta congelado, lo descongelo.
    jmp .desactivarDebug

.activarDebug:
    call debugearJuego
    jmp .fin

.desactivarDebug:
    ; Desactivo el modo debug.
    call noDebugearJuego
    ; Verifico si ademas debo descongelar.
    call esCongelado
    cmp eax, 0
    je .fin
    
    ; Si esta congelado
    ; Volvemos a pintar la pantalla , descongelo.
    call pantallaMagenta

    call resumirJuego
    jmp .fin

.fin:

    call pic_finish1

    popad

    iret

_isr137:
    
    pushad

    xor esi, esi
    ; Cargamos el task register
    str si
    ; Obtenemos el indice en la GDT de la TSS para la tarea que nos llamo.
    shr si, 3    
    ; Restamos 15 para obtener el codigo de la tarea que nos llamo
    sub si, 15
    
    push esi
    push edx
    push ecx
    push ebx
    push eax
    
    call handler137C
    mov BYTE [eax_backup], al
    
    
    add esp, 20      
    popad
    xor eax, eax
    mov al, [eax_backup]
    iret

_isr138:
     pushad
     push esi
     xor esi, esi
        
     ; Cargamos el task register
     str si
     ; Obtenemos el indice en la GDT de la TSS para la tarea que nos llamo.
    shr si, 3    
    ; Restamos 15 para obtener el codigo de la tarea que nos llamo
    sub si, 15
     
     ; Codigo de la tarea
     push esi
     ; Codigo del universo
     push eax
     call handler138C
     add esp, 8
     pop esi
     popad
iret

_isr139: 
     pushad
     push esi
    
     xor esi, esi
    
     

     ; Cargamos el task register
     str si
     ; Obtenemos el indice en la GDT de la TSS para la tarea que nos llamo.
     shr si, 3    
     ; Restamos 15 para obtener el codigo de la tarea que nos llamo
     sub si, 15

     push esi
     push y
     ;push ebx
     push x
     ;push eax
        
     call handler139C

     add esp, 12
         
          
     pop esi
     popad
     mov ebx, [y]
     mov eax, [x]
     
     
iret

global _isr150
_isr150:
    mov eax, 150
    iret

global _isr151
_isr151:
    mov eax, 151
    iret


    

;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
