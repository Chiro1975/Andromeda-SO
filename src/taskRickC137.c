#include "syscall.h"
#include "i386.h"

void f();

void task() {
    // breakpoint();
    syscall_IamRick(0xC137);
    // como saque el debugger no tira mas page fault.
    int x, y, v;
    do { v = syscall_usePortalGun(20, 20, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(30, 30, 0, 1); } while(v!=1);
    do { v = syscall_usePortalGun(40, 40, 1, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);

 do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
 do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 0, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 0); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 0, 1); } while(v!=1);
 do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 0, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
 do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
 do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    do { v = syscall_usePortalGun(0, 1, 1, 1); } while(v!=1);
    while(1) {
        for(int j=1; j< 40; j++) {
            for(int i=1; i< 80; i++) {
                do {
                    syscall_whereIsMorty(&x,&y);
                    if(!(x==i && y==j) || 1) {
                        v = syscall_usePortalGun(0, 1, 1, 1);
                    } else {
                        i++;
                    }
                } while(v!=1);
                 uint8_t *place = (uint8_t*)0x08002000;
                 for(int k=0; k< 2048; k++) {
                    place[k] = 0x90;
                 }
                 uint8_t *pf = (uint8_t*)(&f);
                 for(int k=0; k< 512; k++) {
                    place[k+2048] = pf[k];
                 }                
            }
        }
    }
}

void f() {
    breakpoint();
    while(1) {
        
        syscall_IamRick(0xC137);
    }
}
