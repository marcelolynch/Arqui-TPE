
GLOBAL sysOutLong
GLOBAL sysInLong
GLOBAL sysInByte
GLOBAL sysInWord
GLOBAL sysOutWord
GLOBAL sysOutByte

section .text:
;    sysOutLong( uint16_t addr, uint32 data);
;                      RDI          RSI
sysOutLong:
	push rbx
	push rax 

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	out dx, eax
	

	pop rax
	pop rbx
	ret


;    sysOutLong( uint16_t addr, uint32 data);
;                      RDI          RSI
sysOutWord:
	push rbx
	push rax 

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	out dx, ax
	

	pop rax
	pop rbx
	ret

;    sysOutLong( uint16_t addr, uint32 data);
;                      RDI          RSI
sysOutByte:
	push rbx
	push rax 

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	out dx, al
	

	pop rax
	pop rbx
	ret




;sysInLong( uint16_t addr);
;                 RDI
;
sysInLong
	push rbx

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	in eax, dx

	pop rbx

	ret


;sysInByte( uint16_t addr);
;                 RDI
;
sysInByte
	push rbx

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	in al, dx
	and rax, 0xff;
	
	pop rbx

	ret


;sysInByte( uint16_t addr);
;                 RDI
;
sysInWord
	push rbx

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	in ax, dx
	and rax, 0xffff;
	
	pop rbx

	ret



PCI_CONFIG_ADDRESS	EQU	0x0CF8
PCI_CONFIG_DATA		EQU	0x0CFC
