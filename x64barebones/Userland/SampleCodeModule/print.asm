GLOBAL puts
section .text

sys_read:
	mov rbx, 1 ;STDOUT
	mov rcx, rdi ;string
	mov rdx, rsi ;size

	mov rax, 4 ;Write (!?)
	int 80h

	ret