GLOBAL cpuVendor
GLOBAL sti
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL irq11Handler
GLOBAL irq8Handler

EXTERN irqDispatcher

%include "./asm/macros.m"

section .text

irq0Handler:
	irqHandler 0
	
irq1Handler:
	irqHandler 1

irq8Handler:
	irqHandler 8

irq11Handler:
	irqHandler 11

sti:
	sti
	ret
	
setPicMaster:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 21h, al
	call outportb
	
	mov rsp, rbp
	pop rbp
	ret


setPicSlave:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; en al esa la mascara
    out	    0A1h, al
    pop     rbp
    retn

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


;Recibo en el primer parametro (RDI) el puerto de salida
;En el segundo (RSI) el dato a escribir (se escribe el byte menos significativo)
outportb:
	mov rax, rsi
	mov rdx, rdi
	out dx, al
	ret